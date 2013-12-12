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
require_once "config.inc.php";

$mysqli = NULL;

function db_connect()
{
        global $dbhost, $dbuser, $dbpass, $database, $mysqli;
        
        $mysqli = new mysqli($dbhost, $dbuser, $dbpass, $database);
        if ($mysqli->connect_errno) internalServerError("Database connection failed");
}

function db_close()
{
        global $mysqli;
        if ($mysqli) {
                $mysqli->close();
                $mysqli = NULL;
        }
}

?>
