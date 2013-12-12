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
require_once "account.inc.php";
require_once "group.inc.php";
require_once "photostream.inc.php";

db_connect();

$key = input_get_key();
if (!is_valid_key($key)) badRequest("Invalid key");

$account = new Account();
if (!$account->load("key", $key)) 
        badRequest("Invalid key");
if (!$account->validated) 
        unauthorized("Invalid account");

$json = file_get_contents('php://input');
$input = json_decode($json);
if (!$input) {
        $msg = "";
        switch (json_last_error()) {
        case JSON_ERROR_NONE:
                $msg = 'The JSON decoded returned an empty object';
                break;
        case JSON_ERROR_DEPTH:
                $msg = 'JSON decoder: Maximum stack depth exceeded';
                break;
        case JSON_ERROR_STATE_MISMATCH:
                $msg = 'JSON decoder: Underflow or the modes mismatch';
                break;
        case JSON_ERROR_CTRL_CHAR:
                $msg = 'JSON decoder: Unexpected control character found';
                break;
        case JSON_ERROR_SYNTAX:
                $msg = 'JSON decoder: Syntax error, malformed JSON';
                break;
        case JSON_ERROR_UTF8:
                $msg = 'JSON decoder: Malformed UTF-8 characters, possibly incorrectly encoded';
                break;
        default:
                $msg = 'JSON decoder: Unknown error';
                break;
        }
        badRequest($msg);    
}

$photostream = new Photostream();

if ($input->id) {
        if (!$photostream->load($input->id)) 
                badRequest("Invalid ID: " . $input->id);
        if ($photostream->owner != $account->id) 
                unauthorized("You don't have the required privileges to change datastream " . $input->id);
} else {
        $photostream->owner = $account->id;
        if (!isset($input->name)) 
                badRequest("Invalid name: " . $input->name);
}
if (isset($input->name)) {
        if (!is_valid_identifier($input->name))
                badRequest("Invalid name: " . $input->name);
        $photostream->name = $input->name;
}
if (isset($input->description)) {
        $photostream->description = $input->description;
}
if (isset($input->timezone)) {
        if (!is_numeric($input->timezone))
                badRequest("Invalid timezone: " . $input->timezone);
        $photostream->timezone = $input->timezone;
}
if (isset($input->longitude)) {
        if (!is_numeric($input->longitude))
                badRequest("Invalid longitude: " . $input->longitude);
        $photostream->longitude = $input->longitude;
}
if (isset($input->latitude)) {
        if (!is_numeric($input->latitude))
                badRequest("Invalid latitude: " . $input->latitude);
        $photostream->latitude = $input->latitude;
}
if (isset($input->unit)) {
        if (!is_valid_unit($input->unit))
                badRequest("Invalid unit: " . $input->unit);
        $photostream->unit = $input->unit;
}

if ($photostream->id) {
        if (!$photostream->update())
                internalServerError("Update of photostream failed");
        /*
} else if ($photostream->loadIdUsingOwnerAndName($account->id, $photostream->name)) {
        if (!$photostream->update())
                internalServerError("Update of photostream failed");
        */
} else {
        if (!$photostream->create())
                internalServerError("Failed to create the photostream");
}

db_close();

header('Content-type: application/json');
echo json_encode($photostream) . "\n";

?>
