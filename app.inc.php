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

require_once "error.inc.php";
require_once "db.inc.php";
require_once "input.inc.php";

class App
{
        public $id;
        public $owner;
        public $name;
        public $description;
        public $url;

        public function load($what, $value) 
        {
                global $mysqli;

                $query = ("SELECT id, owner, name, description, url FROM `app` ".
                          "WHERE `" . $what . "`='" . $mysqli->real_escape_string($value) . "';");
                //echo $query;

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) 
                        return false;

                $this->import($res->fetch_assoc());

                return true;
        }

        public function import($row) 
        {
                $this->id = $row['id'];
                $this->owner = $row['owner'];
                $this->name = $row['name'];
                $this->description = $row['description'];
                $this->url = $row['url'];
        }

        public function create($key) 
        {
                global $mysqli;
                
                if (isset($this->id)) 
                        return false;

                $query = "INSERT INTO `app` VALUES (" 
                        . "NULL," 
                        . $mysqli->real_escape_string($this->owner) . ","
                        . "'" . $mysqli->real_escape_string($key) . "',"
                        . "'" . $mysqli->real_escape_string($this->name) . "',"
                        . "'" . $mysqli->real_escape_string($this->description) . "',"
                        . "'" . $mysqli->real_escape_string($this->url) . "');";
                
                //echo $query . "\n"; 
                /* return true; */


                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        //echo $mysqli->error . "\n";
                        return false;
                }

                $this->id = $mysqli->insert_id;

                return true;
        }

        public function linkAccount($account) 
        {
                global $mysqli;
                
                $query = "INSERT INTO `app_account` VALUES (" 
                        . $this->id . ","
                        . $account->id . ");";
                
                //echo $query . "\n"; 
                /* return true; */

                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        //echo $mysqli->error . "\n";
                        return false;
                }

                return true;
        }

        public function update() 
        {
                global $mysqli;

                if (!isset($this->id)) 
                        return false;

                $query = "UPDATE `group` SET " 
                        . "name='" . $mysqli->real_escape_string($this->name) . "',"
                        . "description='" . $mysqli->real_escape_string($this->description) . "'"
                        . "url='" . $mysqli->real_escape_string($this->url) . "'"
                        . "WHERE `id`=" . $mysqli->real_escape_string($this->id) . ";";

                //echo $query . "\n";
                /* return true; */


                $res = $mysqli->query($query);

                return ($mysqli->errno == 0);
        }
}

?>
