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

require_once "datetime.model.php";

// ID.ext
// ID/yearmonthday[-hourminutesecond].ext
// ID/yearmonthday[-hourminutesecond]/yearmonthday[-hourminutesecond].ext

class Selector
{
        public $id;
        public $date;
        public $from;
        public $to;
        public $format;
        public $size;
        public $err;

        public function parse($path) 
        {
                $this->id = NULL;
                $this->date = NULL;
                $this->from = NULL;
                $this->to = NULL;
                $this->format = NULL;
                $this->err = NULL;

                if (strlen($path) < 5) {
                        $this->err = "Invalid selection: " . $path;
                        return false;
                }

                $ext = substr($path, -4);
                if ($ext == ".csv") {
                        $this->format = "csv";
                        $s = substr($path, 0, -4);
                } else if ($ext == "json") {
                        $this->format = "json";
                        $s = substr($path, 0, -5);
                } else if ($ext == ".txt") {
                        $this->format = "txt";
                        $s = substr($path, 0, -4);
                } else if ($ext == ".gif") {
                        $this->format = "gif";
                        $s = substr($path, 0, -4);
                } else {
                        $this->err = "Unrecognised format: " . $path;
                        return false;
                }

                $parts = explode("/", $s);

                $num = count($parts);
                if ($num == 0) {
                        $this->err = "Invalid selection: " . $path;
                        return false;
                }

                if (!is_numeric($parts[0])) {
                        $this->err = "Invalid ID: " . $parts[0];
                        return false;
                }
                $this->id = $parts[0];

                if ($num == 1) {
                        // nothing to do
                
                } else if ($num == 2) {
                        $this->date = new OSDDateTime();
                        if (!$this->date->parse($parts[1])) {
                                $this->err = $this->date->err;
                                return false;
                        }

                } else if ($num == 3) {
                        $this->from = new OSDDateTime();
                        if (!$this->from->parse($parts[1])) {
                                $this->err = $this->from->err;
                                return false;
                        }
                        $this->to = new OSDDateTime();
                        if (!$this->to->parse($parts[2])) {
                                $this->err = $this->to->err;
                                return false;
                        }

                } else {
                        $this->err = "Invalid datastream selection: " . $path;
                        return false;
                }

                return true;                
        }

        public function parse_photo($path) 
        {
                if (strlen($path) < 20) {
                        $this->err = "Invalid photo selection: " . $path;
                        return false;
                }

                $parts = explode("/", $path);

                $num = count($parts);

                if (!is_numeric($parts[0]) 
                    || ($num != 2)) {
                        $this->err = "Invalid photo ID (1): " . $path;
                        return false;
                }
                $this->id = $parts[0];

                if (!preg_match('/^[0-9]{8,8}-[0-9]{6,6}[_omst]{0,2}\.jpg$/', $parts[1])) {
                        $this->err = "Invalid photo ID (2): " . $path;
                        return false;
                }

                $ext = substr($parts[1], -4);
                if ($ext != ".jpg") {
                        $this->err = "Invalid photo format: " . $path;
                        return false;
                }

                $this->format = "jpg";

                $date = substr($parts[1], 0, 15);
                $this->date = new OSDDateTime();
                if (!$this->date->parse($date)) {
                        $this->err = $this->date->err;
                        return false;
                }

                $size = substr($parts[1], 15, 2);
                $this->size = "orig";
                if ($size == "_s") 
                        $this->size = "small";
                else if ($size == "_t") 
                        $this->size = "thumb";
                else if ($size == "_m") 
                        $this->size = "medium";

                return true;
        }

        public function join($prefix, $separator, $id, $format)
        {
                if ($id) {
                        $s = $prefix . $separator . $id;
                } else {
                        $s = $prefix . $separator . $this->id;
                }

                if ($this->date != NULL) {
                        if ($this->date->has_time()) {
                                $s .= $separator . $this->date->format("Ymd-HMS");
                        } else {
                                $s .= $separator . $this->date->format("Ymd");
                        }

                } else if (($this->from != NULL) 
                           && ($this->to != NULL)) {
                        if ($this->from->has_time()) {
                                $s .= $separator . $this->from->format("Ymd-HMS");
                        } else {
                                $s .= $separator . $this->from->format("Ymd");
                        }
                        if ($this->to->has_time()) {
                                $s .= $separator . $this->to->format("Ymd-HMS");
                        } else {
                                $s .= $separator . $this->to->format("Ymd");
                        }
                }

                if ($format) {
                        $s .= $format;
                } else {
                        $s .= "." . $this->format;
                }

                return $s;
        }

        public function to_filename($prefix)
        {
                return $this->join($prefix, "_", false, false);
        }

        public function to_path()
        {
                return $this->join("", "/", false, false);
        }

        public function construct_path($id, $format)
        {
                return $this->join("", "/", $id, $format);
        }
}

?>
