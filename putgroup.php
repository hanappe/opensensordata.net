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
require_once "datastream.inc.php";
require_once "photostream.inc.php";

db_connect();

$key = input_get_key();
if (!is_valid_key($key)) badRequest();

$account = new Account();
if (!$account->load("key", $key)) 
        badRequest();
if (!$account->validated) 
        unauthorized("Invalid account");

$json = file_get_contents('php://input');
$input = json_decode($json);
if (!$input) {
        $msg = "";
        switch (json_last_error()) {
        case JSON_ERROR_NONE:
                $msg = 'The JSON decoder returned an empty object';
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

$group = new Group();

if (isset($input->id)) {
        if (!$group->load($input->id, true)) 
                badRequest();
        if ($group->owner != $account->id) 
                unauthorized("Please verify that your are the owner of group " . $input->id);        
} else {
        $group->owner = $account->id;
        if (!isset($input->name)) 
                badRequest();
}

if (isset($input->name)) {
        if (!is_valid_identifier($input->name))
                badRequest();
        $group->name = $input->name;
}

if (isset($input->description)) {
        $group->description = $input->description;
}

if ($group->id) {
        if (!$group->update()) 
                internalServerError("Update failed");
} else if ($group->loadIdUsingOwnerAndName($account->id, $group->name)) {
        if (!$group->update()) 
                internalServerError("Update failed");
} else if (!$group->create()) {
        internalServerError("Creation failed");
}

if (isset($input->datastreams)) {
        if (!is_array($input->datastreams))
                badRequest("Invalid description of the datastreams");
        
        for ($i = 0; $i < count($input->datastreams); $i++) {
                $d = $input->datastreams[$i];
                $datastream = new Datastream();

                if ($d->id) {
                        if (!$datastream->load($d->id)) 
                                badRequest("Invalid ID: " . $d->id);
                } else {
                        $datastream->owner = $account->id;
                        if (!isset($d->name)) 
                                badRequest("Missing name: datastream " . $i);
                }
                if (isset($d->name)) {
                        if (!is_valid_identifier($d->name))
                                badRequest("Invalid name: " . $d->name);
                        if ($datastream->owner == $account->id) 
                                $datastream->name = $d->name;
                        else if ($d->name != $datastream->name)
                                unauthorized("You don't have the required privileges to change the datastream description");
                }
                if (isset($d->description)) {
                        if ($datastream->owner == $account->id) 
                                $datastream->description = $d->description;
                        else if ($d->description != $datastream->description)
                                unauthorized("You don't have the required privileges to change the datastream description");
                }
                if (isset($d->timezone)) {
                        if (!is_numeric($d->timezone))
                                badRequest("Invalid timezone: " . $d->timezone);
                        if ($datastream->owner == $account->id) 
                                $datastream->timezone = $d->timezone;
                        else if ($d->timezone != $datastream->timezone)
                                unauthorized("You don't have the required privileges to change the datastream description");
                }
                if (isset($d->longitude)) {
                        if (!is_numeric($d->longitude))
                                badRequest("Invalid longitude: " . $d->longitude);
                        if ($datastream->owner == $account->id) 
                                $datastream->longitude = $d->longitude;
                        else if ($d->longitude != $datastream->longitude)
                                unauthorized("You don't have the required privileges to change the datastream description");
                }
                if (isset($d->latitude)) {
                        if (!is_numeric($d->latitude))
                                badRequest("Invalid latitude: " . $d->latitude);
                        if ($datastream->owner == $account->id) 
                                $datastream->latitude = $d->latitude;
                        else if ($d->latitude != $datastream->latitude)
                                unauthorized("You don't have the required privileges to change the datastream description");
                }
                if (isset($d->unit)) {
                        if (!is_valid_unit($d->unit))
                                badRequest("Invalid unit: " . $d->unit);
                        if ($datastream->owner == $account->id) 
                                $datastream->unit = $d->unit;
                        else if ($d->latitude != $datastream->latitude)
                                unauthorized("You don't have the required privileges to change the datastream description");
                }

                if ($datastream->id) {
                        // Update the datastream only if it is owned
                        // by the current client. It's not an error if
                        // the datastream is not owned by the current
                        // client, because it is possible to create
                        // groups that combine datastreams from
                        // different people.
                        if ($datastream->owner == $account->id) {
                                if (!$datastream->update())
                                        internalServerError("Update of datastream failed: "
                                                            . $datastream->name);
                        }
                } else if ($datastream->loadIdUsingOwnerAndName($account->id, $datastream->name)) {
                        if (!$datastream->update())
                                internalServerError("Update of datastream failed: "
                                                    . $datastream->name);
                } else {
                        if (!$datastream->create())
                                internalServerError("Failed to create the datastream " 
                                                    . $datastream->name);
                }

                // Store it for later use.
                $input->datastreams[$i] = $datastream;
        }

        // (Re-)Attach the datastreams to the group.
        if (!$group->remove_datastreams()) 
                internalServerError("Failed to remove the datastreams");
        
        for ($i = 0; $i < count($input->datastreams); $i++) {
                $datastream = $input->datastreams[$i];

                if (!$group->add_datastream($datastream, $i)) 
                        internalServerError("Failed to add a datastream to the group");
        } 
}


if (isset($input->photostreams)) {
        if (!is_array($input->photostreams))
                badRequest("Invalid description of the photostreams");
        
        for ($i = 0; $i < count($input->photostreams); $i++) {
                $d = $input->photostreams[$i];
                $photostream = new Photostream();

                if ($d->id) {
                        if (!$photostream->load($d->id)) 
                                badRequest("Invalid ID: " . $d->id);
                } else {
                        $photostream->owner = $account->id;
                        if (!isset($d->name)) 
                                badRequest("Missing name: photostream " . $i);
                }
                if (isset($d->name)) {
                        if (!is_valid_identifier($d->name))
                                badRequest("Invalid name: " . $d->name);
                        if ($photostream->owner == $account->id) 
                                $photostream->name = $d->name;
                        else if ($d->name != $photostream->name)
                                unauthorized("You don't have the required privileges to change the photostream description");
                }
                if (isset($d->description)) {
                        if ($photostream->owner == $account->id) 
                                $photostream->description = $d->description;
                        else if ($d->description != $photostream->description)
                                unauthorized("You don't have the required privileges to change the photostream description");
                }
                if (isset($d->timezone)) {
                        if (!is_numeric($d->timezone))
                                badRequest("Invalid timezone: " . $d->timezone);
                        if ($photostream->owner == $account->id) 
                                $photostream->timezone = $d->timezone;
                        else if ($d->timezone != $photostream->timezone)
                                unauthorized("You don't have the required privileges to change the photostream description");
                }
                if (isset($d->longitude)) {
                        if (!is_numeric($d->longitude))
                                badRequest("Invalid longitude: " . $d->longitude);
                        if ($photostream->owner == $account->id) 
                                $photostream->longitude = $d->longitude;
                        else if ($d->longitude != $photostream->longitude)
                                unauthorized("You don't have the required privileges to change the photostream description");
                }
                if (isset($d->latitude)) {
                        if (!is_numeric($d->latitude))
                                badRequest("Invalid latitude: " . $d->latitude);
                        if ($photostream->owner == $account->id) 
                                $photostream->latitude = $d->latitude;
                        else if ($d->latitude != $photostream->latitude)
                                unauthorized("You don't have the required privileges to change the photostream description");
                }

                if ($photostream->id) {
                        // Update the photostream only if it is owned
                        // by the current client. It's not an error if
                        // the photostream is not owned by the current
                        // client, because it is possible to create
                        // groups that combine photostreams from
                        // different people.
                        if ($photostream->owner == $account->id) {
                                if (!$photostream->update())
                                        internalServerError("Update of photostream failed: "
                                                            . $photostream->name);
                        }

                } else if ($photostream->loadIdUsingOwnerAndName($account->id, $photostream->name)) {
                        if (!$photostream->update())
                                internalServerError("Update of photostream failed: "
                                                    . $photostream->name);
                } else {
                        if (!$photostream->create())
                                internalServerError("Failed to create the photostream " 
                                                    . $photostream->name);
                }

                // Store it for later use.
                $input->photostreams[$i] = $photostream;
        }

        // (Re-)Attach the photostreams to the group.
        if (!$group->remove_photostreams()) 
                internalServerError("Failed to remove the photostreams");
        
        for ($i = 0; $i < count($input->photostreams); $i++) {
                $photostream = $input->photostreams[$i];

                if (!$group->add_photostream($photostream, $i)) 
                        internalServerError("Failed to add a photostream to the group");
        } 
}


db_close();

header('Content-type: application/json');
echo json_encode($group) . "\n";

?>
