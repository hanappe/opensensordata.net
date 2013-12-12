<?php
/*  
    OpenSensorData.net 

    OpenSensorData.net is a free (as in freedom) online database and
    API to store sensor data.

    Copyright (C) 2013  Sony Computer Science Laboratory Paris
    Author: Peter Hanappe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

class OSDDateTime
{
        public $year;
        public $month;
        public $day;
        public $hour;
        public $minute;
        public $second;
        public $timezone;
        public $err;

        function __construct() 
        {
                $this->year = NULL;
                $this->month = NULL;
                $this->day = NULL;
                $this->hour = NULL;
                $this->minute = NULL;
                $this->second = NULL;
                $this->timezone = NULL;
                $this->err = NULL;
        }

        function number($s, $min, $max, $name)
        {
                if (!is_numeric($s)) {
                        $this->err = "Expected a number for $name";
                        return false;
                }
                $n = (int) $s;
                if (($n < $min) || ($n > $max)) {
                        $this->err = "Invalid $name: $s";
                        return false;
                }
                return $n;
        }

        public function import($row)
        {
                $phpdate = strtotime($row['datetime']);
                $this->year = date('Y', $phpdate);
                $this->month = date('m', $phpdate);
                $this->day = date('d', $phpdate);
                $this->hour = date('H', $phpdate);
                $this->minute = date('i', $phpdate);
                $this->second = date('s', $phpdate);
        }

        public function parse($s)
        {
                $parts = explode("-", $s);

                $n = count($parts);
                if (($n != 1) && ($n != 2)) {
                        $this->err = "Invalid datetime: " . $s;
                        return false;
                }

                $d = $parts[0];
                if (strlen($d) != 8) {
                        $this->err = "Invalid date: " . $d;
                        return false;
                }
                $this->year = $this->number(substr($d, 0, 4), 1900, 2100, "year");
                $this->month = $this->number(substr($d, 4, 2), 1, 12, "month");
                $this->day = $this->number(substr($d, 6, 2), 1, 31, "day");

                if ($n == 2) {
                        $t = $parts[1];
                        if (strlen($t) != 6) {
                                $this->err = "Invalid time: " . $t;
                                return false;
                        }
                        $this->hour = $this->number(substr($t, 0, 2), 0, 23, "hour");
                        $this->minute = $this->number(substr($t, 2, 2), 0, 59, "minute");
                        $this->second = $this->number(substr($t, 4, 2), 0, 59, "second");
                } 
                return $this->err? false : true;
        }

        public function has_time()
        {
                return $this->hour != NULL;
        }

        public static function _format($format, 
                                       $year, $month, $day, 
                                       $hour, $minute, $second, 
                                       $timezone)
        {
                if ($format == "iso8601") 
                        return sprintf("%04d-%02d-%02dT%02d:%02d:%02dZ",
                                       $year, $month, $day, 
                                       $hour, $minute, $second);

                else if ($format == "Y/m/d H:M:S") 
                        return sprintf("%04d/%02d/%02d %02d:%02d:%02d",
                                       $year, $month, $day, 
                                       $hour, $minute, $second);

                else if ($format == "Y-m-d H:M:S") 
                        return sprintf("%04d-%02d-%02d %02d:%02d:%02d",
                                       $year, $month, $day, 
                                       $hour, $minute, $second);

                else if ($format == "Ymd-HMS") 
                        return sprintf("%04d%02d%02d-%02d%02d%02d",
                                       $year, $month, $day, 
                                       $hour, $minute, $second);

                else if ($format == "Y-m-d") 
                        return sprintf("%04d-%02d-%02d", $year, $month, $day);

                else if ($format == "Ymd") 
                        return sprintf("%04d%02d%02d", $year, $month, $day);

                return "BADTIMEFORMAT";
        }

        public function format($format)
        {
                return OSDDateTime::_format($format,
                                            $this->year, $this->month, $this->day, 
                                            $this->hour, $this->minute, $this->second,
                                            $this->timezone);
        }
}

?>
