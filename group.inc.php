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
require_once "datastream.inc.php";
require_once "photostream.inc.php";

class Group
{
        public $id;
        public $owner;
        public $app;
        public $name;
        public $description;
        public $datastreams = array();
        public $photostreams = array();

        public function load($id, $load_streams) 
        {
                global $mysqli;

                $query = "SELECT id, owner, app, name, description FROM `group` WHERE `id`=" . $mysqli->real_escape_string($id) . ";";
                //echo $query;

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) 
                        return false;

                $this->import($res->fetch_assoc());

                if ($load_streams) {
                        $this->loadDatastreams();
                        $this->loadPhotostreams();
                }

                return true;
        }

        public function import($row) 
        {
                $this->id = $row['id'];
                $this->owner = $row['owner'];
                $this->app = $row['app'];
                $this->name = $row['name'];
                $this->description = $row['description'];
        }

        public function loadDatastreams() 
        {
                global $mysqli;

                $query = "SELECT datastream FROM `group_datastreams` WHERE `gid`=" . $mysqli->real_escape_string($this->id) . " ORDER BY `index`;";
                //echo $query;
                
                $res = $mysqli->query($query);
                if (!$res) {
                        //echo $mysqli->error;
                        return false;
                }
                
                while ($row = $res->fetch_assoc()) {
                        $datastream = new Datastream();
                        if (!$datastream->load($row['datastream']))
                                return false;
                        $this->datastreams[] = $datastream;
                }
        }

        public function loadPhotostreams() 
        {
                global $mysqli;

                $query = "SELECT photostream FROM `group_photostreams` WHERE `gid`=" . $mysqli->real_escape_string($this->id) . " ORDER BY `index`;";
                //echo $query;
                
                $res = $mysqli->query($query);
                if (!$res) {
                        //echo $mysqli->error;
                        return false;
                }
                
                while ($row = $res->fetch_assoc()) {
                        $photostream = new Photostream();
                        if (!$photostream->load($row['photostream']))
                                return false;
                        $this->photostreams[] = $photostream;
                }
        }

        public function loadIdUsingOwnerAndName($owner, $name) 
        {
                global $mysqli;

                $query = "SELECT `id` FROM `group` "
                        . "WHERE `owner`=" . $mysqli->real_escape_string($owner) 
                        . " AND `name`='" . $mysqli->real_escape_string($name) . "';";
                //echo $query;

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) 
                        return false;

                $row = $res->fetch_assoc();

                $this->id = $row['id'];

                return true;
        }

        public function create() 
        {
                global $mysqli;
                
                if (isset($this->id)) 
                        return false;

                $app = ($this->app)? $this->app : "NULL";

                $query = "INSERT INTO `group` VALUES (" 
                        . "NULL," 
                        . $mysqli->real_escape_string($this->owner) . ","
                        . $mysqli->real_escape_string($app) . ","
                        . "'" . $mysqli->real_escape_string($this->name) . "',"
                        . "'" . $mysqli->real_escape_string($this->description) . "');";
                
                // echo $query . "\n"; 
                /* return true; */


                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        //echo $mysqli->error . "\n";
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

                $query = "UPDATE `group` SET " 
                        . "name='" . $mysqli->real_escape_string($this->name) . "',"
                        . "description='" . $mysqli->real_escape_string($this->description) . "'"
                        . "WHERE `id`=" . $mysqli->real_escape_string($this->id) . ";";

                //echo $query . "\n";
                /* return true; */


                $res = $mysqli->query($query);

                return ($mysqli->errno == 0);
        }

        public function remove_datastreams() 
        {
                global $mysqli;

                if (!isset($this->id)) 
                        return false;

                $this->datastreams = array();
                
                $query = "DELETE FROM `group_datastreams` "
                        . "WHERE `gid`=" . $mysqli->real_escape_string($this->id) . ";";
                 $mysqli->query($query);
                return true;
        }

        public function add_datastream($d, $index) 
        {
                global $mysqli;

                if (!isset($this->id)) 
                        return false;

                $this->datastreams[] = $d;

                // FIXME: Use update...
                
                $query = "INSERT INTO `group_datastreams` VALUES ("
                        . $mysqli->real_escape_string($this->id) . ","
                        . $mysqli->real_escape_string($index) . ","
                        . $mysqli->real_escape_string($d->id) . ");";
 
                /* echo $query . "\n"; */
                /* return true; */


                $res = $mysqli->query($query);
                
                return ($mysqli->errno == 0);
        }

        public function count_datastreams() 
        {
                return count($this->datastreams);
        }

        public function get_datastream($i) 
        {
                return $this->datastreams[$i];
        }

        public function remove_photostreams() 
        {
                global $mysqli;

                if (!isset($this->id)) 
                        return false;

                $this->photostreams = array();
                
                $query = "DELETE FROM `group_photostreams` "
                        . "WHERE `gid`=" . $mysqli->real_escape_string($this->id) . ";";
                 $mysqli->query($query);
                return true;
        }

        public function add_photostream($d, $index) 
        {
                global $mysqli;

                if (!isset($this->id)) 
                        return false;

                $this->photostreams[] = $d;

                // FIXME: Use update...
                
                $query = "INSERT INTO `group_photostreams` VALUES ("
                        . $mysqli->real_escape_string($this->id) . ","
                        . $mysqli->real_escape_string($index) . ","
                        . $mysqli->real_escape_string($d->id) . ");";
 
                /* echo $query . "\n"; */
                /* return true; */


                $res = $mysqli->query($query);
                
                return ($mysqli->errno == 0);
        }

        public function count_photostreams() 
        {
                return count($this->photostreams);
        }

        public function get_photostream($i) 
        {
                return $this->photostreams[$i];
        }
}

?>
