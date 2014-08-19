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
require_once "photostream.inc.php";

$path = $_GET['path'];

$selector = new Selector();
if (!$selector->parse_photo($path))
        badRequest($selector->err);

$file = Photostream::selector_to_path($photodir, $selector, NULL);

if (!file_exists($file))
        notFound();

header('Content-Type: image/jpeg');
header('Content-Length: ' . filesize($file));
if (!readfile($file))
        internalServerError("Failed to read the file.");

?>
