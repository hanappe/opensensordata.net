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

load_account();

$selected_username = $_REQUEST['name'];
if (!isset($selected_username)) {
        if (isset($account)) {
                $selected_username = $account->username;
        } else {
                badRequest("Please send a valid account ID");
        }
} else {
        $selected_username = trim($selected_username);
        if (!is_valid_username($selected_username)) {
                badRequest("Please check the username: " . $selected_username);
        }
 }


db_connect();

$selected_account = new Account();
if (!$selected_account->load("username", $selected_username)) {
        badRequest("Please check the username: " . $selected_username);
}

$groups = $selected_account->loadGroups();
$datastreams = $selected_account->loadDatastreams();
$photostreams = $selected_account->loadPhotostreams();

if ($_REQUEST['format'] == 'html') {
        require_once "account_view.inc.php";
 } else {
        $output = array("id" => $selected_account->id,
                        "username" => $selected_account->username,
                        "groups" => $groups,
                        "datastreams" => $datastreams,
                        "photostreams" => $photostreams);
        echo json_encode($output);
 } 

db_close();

?>
