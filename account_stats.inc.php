<?php /* 

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

require_once "error.inc.php";
require_once "db.inc.php";
require_once "input.inc.php";

class AccountStats
{
        public $id;
        public $data_maxuploads = 0;
        public $data_uploads = 0;
        public $data_curweek = 0;
        public $photo_maxuploads;
        public $photo_uploads = 0;
        public $photo_curweek = 0;

        public function load($id) 
        {
                global $mysqli;

                $query = ("SELECT * FROM account_stats WHERE " 
                          . "`aid`='" . $mysqli->real_escape_string($id) . "'");

                //echo $query . "<br/>\n";

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) return false;
                $row = $res->fetch_assoc();

                $this->id = $row['aid'];
                $this->data_maxuploads = $row['data_maxuploads'];
                $this->data_uploads = $row['data_uploads'];
                $this->data_curweek = $row['data_curweek'];
                $this->photo_maxuploads = $row['photo_maxuploads'];
                $this->photo_uploads = $row['photo_uploads'];
                $this->photo_curweek = $row['photo_curweek'];

                return true;
        }
        
        public function create($id) 
        {
                global $mysqli, $max_weekly_photo_uploads, $max_weekly_data_uploads;
                
                $this->data_maxuploads = $max_weekly_data_uploads;
                $this->photo_maxuploads = $max_weekly_photo_uploads;

                $query = ("INSERT INTO account_stats VALUES (" 
                          . $mysqli->real_escape_string($id) 
                          . $mysqli->real_escape_string($this->data_maxuploads) 
                          . ", 0, 0, " 
                          . $mysqli->real_escape_string($this->photo_maxuploads) 
                          . ", 0, 0);");

                //echo $query . "<br/>\n";

                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        //echo $mysqli->error . "<br/>\n";
                        return false;
                }

                $this->id = $mysqli->insert_id;
                return true;
        }

        public function update() 
        {
                global $mysqli;
                
                if (!isset($this->id)) 
                        return false;

                $query = ("UPDATE account_stats SET "
                          . "data_maxuploads=" . $this->data_maxuploads . ", "
                          . "data_uploads=" . $this->data_uploads . ", "
                          . "data_curweek=" . $this->data_curweek . ", "
                          . "photo_maxuploads=" . $this->photo_maxuploads . ", "
                          . "photo_uploads=" . $this->photo_uploads . ", "
                          . "photo_curweek=" . $this->photo_curweek
                          . " WHERE aid=" . $this->id . ";");
                //echo $query . "<br/>\n";

                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        //echo $mysqli->error . "<br/>\n";
                        return false;
                }

                return true;
        }
}

?>
