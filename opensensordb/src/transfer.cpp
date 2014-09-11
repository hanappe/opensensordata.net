#include <iostream>
#include <sstream>
#include <limits>
#include <ctime>
#include <assert.h>
#include <mysql/mysql.h>
#include "osdb_priv.h"

using namespace std;

static datastream_t* last_datastream = 0;
static ulong last_datastream_id = std::numeric_limits<ulong>::max();
static const bool debug = false;

static datastream_t* get_datastream(ulong id)
{
        if (id == last_datastream_id) {
                return last_datastream;
        }

        if (last_datastream != 0) {
                datastream_store(last_datastream);
                delete_datastream(last_datastream);
                last_datastream = 0;
                last_datastream_id = std::numeric_limits<ulong>::max();
        }

        datastream_t* d = datastream_load(id);
        if (d == 0) {
                d = datastream_create(id, 0.5, 3600.0, 1.1);
                if (d == 0) {
                        cerr << "datastream_create returned 0" << endl;
                        return 0;
                }
        }

        last_datastream_id = id;
        last_datastream = d;

        return last_datastream;
}

int main(int argc, char** argv)
{
        int res = 0;
        osdb_t* osdb = 0;
        MYSQL* mysql = 0;

        ulong limit_start = 0;
        ulong limit_count = 0;

        if (argc > 3) {
                cerr << "Usage: " << argv[0] << " [limit start (default 0)]  [limit count (default 1000)]" << endl;
                res = -1;
                goto main_end;
        }

        if (argc > 1) {
                limit_start = stoul(argv[1]);
        }

        if (argc > 2) {
                limit_count = stoul(argv[2]);
        }

        {
                osdb = osdb_init();
                osdb_set_option(osdb, "dir", "/tmp/opensensordata");
        }

        {
                mysql = mysql_init(0);
                if (mysql == 0) {
                        cerr << "mysql_init error" << endl;
                        res = -1;
                        goto main_end;
                }

                if (!mysql_real_connect(mysql, "localhost", "opensensordata", "xtestX", "opensensordata", 0, 0, 0)) {
                        cerr << "mysql_real_connect: " << mysql_error(mysql) << endl;
                        res = -1;
                        goto main_end;
                }

                stringstream ss;
                ss << "SELECT `datastream`, `datetime`, `value` FROM `datapoints`";
                if (limit_count > 0) {
                        ss << " LIMIT " << limit_start << ", " << limit_count;
                }
                ss << ";";

                if (mysql_query(mysql, ss.str().data()) != 0) {
                        cerr << "mysql_query: " << mysql_error(mysql) << endl;
                        res = -1;
                        goto main_end;
                }

                MYSQL_RES* myres = mysql_use_result(mysql);
                if (myres == 0) {
                        cerr << "mysql_use_result: " << mysql_error(mysql) << endl;
                        res = -1;
                        goto main_end;
                }

                MYSQL_ROW myrow = 0;
                ulong nrows = 0;
                ulong ndatapoints = 0;
                ulong nerrors = 0;
                ulong nulls = 0;
                ulong badDates = 0;

                tzset();
                time_t cestTimezone = -3600;
                time_t cestOffset = timezone - cestTimezone;
                struct tm tm;
                memset(&tm, 0, sizeof(tm));

                while ((myrow = mysql_fetch_row(myres)) != 0) {
                        nrows++;
                        if (nrows % 1000000 == 0) {
                                cout << (limit_start + nrows) << endl;
                        }

                        if (myrow[0] == 0 || myrow[1] == 0 || myrow[2] == 0) {
                                if (debug) {
                                        cout << "row #" << (limit_start + nrows - 1) << ": "
                                             << ((myrow[0] == 0) ? "null" : myrow[0]) << ", "
                                             << ((myrow[1] == 0) ? "null" : myrow[1]) << ", "
                                             << ((myrow[2] == 0) ? "null" : myrow[2]) << endl;
                                }
                                nulls++;
                                continue;
                        }

                        if (strlen(myrow[1]) != 19) {
                                cout << "bad date format at row #" << (limit_start + nrows - 1) << ": "
                                     << myrow[0] << ", " << myrow[1] << ", " << myrow[2] << endl;
                                badDates++;
                                continue;

                        }

                        ss.clear();
                        ss.str(myrow[1]);
                        int day, month, year, hour, minute, second; char c;
                        ss >> year >> c >> month >> c >> day >> hour >> c >> minute >> c >> second;

                        memset(&tm, 0, sizeof(tm));
                        tm.tm_year = year - 1900;
                        tm.tm_mon = month - 1;
                        tm.tm_mday = day;
                        tm.tm_hour = hour;
                        tm.tm_min = minute;
                        tm.tm_sec = second;
                        timestamp_t time = mktime(&tm) - cestOffset;

                        ulong id = stoul(myrow[0]);
                        value_t value = stod(myrow[2]);


                        if (debug) {
                                cout << "row #" << (limit_start + nrows - 1) << ": "
                                     << myrow[0] << ", "
                                     << myrow[1] << " -> "
                                     << year << '-' << month << '-' << day << ' '
                                     << hour << ':' << minute << ':' << second
                                     << " -> " << (ulong) time << ", "
                                     << myrow[2] << endl;
                        }

                        datastream_t* d = get_datastream(id);
                        if (d == 0) {
                                cerr << "Could not get datastream " << id << "." << endl;
                                cerr << "Exiting..." << endl;
                                break;
                        }

                        if (datastream_insert(d, time, value) != 0) {
                                if (debug) {
                                        cerr << "Could not insert datapoint "
                                             << '[' << id << ", " << (ulong) time << ", " << value << ']'
                                             << " at row " << (limit_start + nrows - 1)
                                             << "." << endl;
                                }
                                nerrors++;
                                continue;
                        }

                        ndatapoints++;
                }

                cout << nrows << " row" << ((nrows == 1) ? "" : "s")
                     << ", " << ndatapoints << " datapoint" << ((ndatapoints == 1) ? "" : "s")
                     << " (" << nerrors << " error" << ((nerrors == 1) ? "" : "s")
                     << ", " << nulls << " null" << ((nulls == 1) ? "" : "s")
                     << ", " << badDates << " bad date" << ((badDates == 1) ? "" : "s") << ")" << endl;
                assert((nrows - ndatapoints) == (nerrors + nulls + badDates));

                mysql_free_result(myres);
        }

 main_end:

        if (mysql != 0) {
                mysql_close(mysql);
                mysql = 0;
        }

        if (last_datastream != 0) {
                datastream_store(last_datastream);
                delete_datastream(last_datastream);
                last_datastream = 0;
                last_datastream_id = std::numeric_limits<ulong>::max();
        }

        if (osdb != 0) {
                osdb_finalize(osdb);
                osdb = 0;
        }

        return res;
}
