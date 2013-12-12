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


function input_get_key()
{
        $headers = getallheaders();
        $key = NULL;
        if (isset($headers['X-OpenSensorData-Key']))
                $key = $headers['X-OpenSensorData-Key'];
        else if (isset($_GET['key']))
                $key = $_GET['key'];
        return $key;
}

function input_get_appkey()
{
        $headers = getallheaders();
        $key = $headers['X-OpenSensorData-AppKey'];
        if (!isset($key)) 
                $key = $_GET['appkey'];
        return $key;
}

function input_get_username()
{
        /* if (isset($_REQUEST['username'])) */
        /*         return preg_replace('/[^\w]/', '', $_REQUEST['username']); */
        /* else return ''; */
        return trim($_REQUEST['username']);
}

function input_get_email()
{
        return trim($_REQUEST['email']);
}

function input_get_password()
{
        return $_REQUEST['pw'];
}

function input_get_pwhash()
{
        return $_REQUEST['pwhash'];
}

function input_get_timeformat()
{
        if (isset($_REQUEST['tf']))
                return $_REQUEST['tf'];
        else return "dygraph";
}

function input_get_geolocated()
{
        if (isset($_REQUEST['geo']))
                return $_REQUEST['geo'];
        else return "no";
}

function input_get_photosize()
{
        if (!isset($_REQUEST['size']))
                return "orig";
        $size = $_REQUEST['size'];
        if (($size != "orig")
            && ($size != "small")
            && ($size != "thumb"))
                badRequest("Invalid photo size");
        return $size;
}

function is_valid_username($s)
{
        return preg_match('/^[a-zA-Z][a-zA-Z0-9_]{2,23}$/', $s);
}

function is_valid_password($s)
{
        $len = strlen($s);
        if ($len < 8) 
                return false;
        if ($len > 256) 
                return false;
        if (preg_match("/^[a-zA-Z]+$/", $s)) // not all characters
                return false;
        if (preg_match("/^[0-9]+$/", $s))  // not all numbers
                return false;
        $c = substr($s, 0, 1);
        $count = 1;
        for ($i = 1; $i < $len; $i++) 
                if (substr($s, $i, 1) == $c)
                        $count++;
        if ($count == $len)
                return false;

        return true;
}

function is_valid_pwhash($s)
{
        return preg_match('/^[a-z0-9]{32,32}$/', $s);
}

function is_valid_key($s)
{
        return preg_match('/^[a-zA-Z0-9]{8,8}-[a-zA-Z0-9]{4,4}-[a-zA-Z0-9]{4,4}-[a-zA-Z0-9]{4,4}-[a-zA-Z0-9]{12,12}$/', $s);
}

function is_valid_identifier($s)
{
        return preg_match('/^[a-zA-Z][a-zA-Z0-9_-]{0,}$/', $s);
}

function is_valid_timeformat($s)
{
        if ($s == "iso8601") return true;
        if ($s == "dygraph") return true;
        return false;
}

function is_valid_unit($s)
{
        return preg_match('#^[a-zA-Z0-9%/]{1,32}$#', $s);
}

function is_valid_email($email)
{
        return filter_var($email, FILTER_VALIDATE_EMAIL);
}

/*

  Accepted IDs:
  - user-id/group-id/stream-index
  - user-id/group-id/stream-name
  - user-id/group-name/stream-index
  - user-id/group-name/stream-name
  - user-name/group-id/stream-index
  - user-name/group-id/stream-name
  - user-name/group-name/stream-index
  - user-name/group-name/stream-name

*/
function get_datastream_id($user, $group, $stream)
{
        global $mysqli;

        $owner_is_id = is_numeric($user);
        $owner_is_name = is_valid_username($user);
        $group_is_id = is_numeric($group);
        $group_is_name = is_valid_identifier($group);
        $stream_is_index = is_numeric($stream);
        $stream_is_name = is_valid_identifier($stream);

        if ($owner_is_id && $group_is_id && $stream_is_index) {
                $query = ("SELECT gd.datastream as id from account a, `group` g, group_datastreams gd "
                          . "WHERE g.owner=" . $user . " " 
                          . "AND g.id=" . $group . " "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_id && $group_is_id && $stream_is_name) {
                $query = ("SELECT gd.datastream as id from account a, `group` g, group_datastreams gd, datastream d "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.id='" . $group . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.datastream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");

        } else if ($owner_is_id && $group_is_name && $stream_is_index) {
                $query = ("SELECT gd.datastream as id from `group` g, group_datastreams gd "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_id && $group_is_name && $stream_is_name) {
                $query = ("SELECT gd.datastream as id from `group` g, group_datastreams gd, datastream d "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.datastream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");
                        
        } else if ($owner_is_name && $group_is_id && $stream_is_index) {
                $query = ("SELECT gd.datastream as id from account a, `group` g, group_datastreams gd "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.id='" . $group . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_name && $group_is_id && $stream_is_name) {
                $query = ("SELECT gd.datastream as id from account a, `group` g, group_datastreams gd, datastream d "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.id=" . $group . " "
                          . "AND g.id=gd.gid "
                          . "AND gd.datastream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");

        } else if ($owner_is_name && $group_is_name && $stream_is_index) {
                $query = ("SELECT gd.datastream as id from account a, `group` g, group_datastreams gd "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_name && $group_is_name && $stream_is_name) {
                $query = ("SELECT gd.datastream as id from account a, `group` g, group_datastreams gd, datastream d "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.datastream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");

        } else {
                badRequest("Invalid datastream ID: " . implode("/", $a));
        }

        //echo $query . "\n";

        $res = $mysqli->query($query);
        if (!$res) internalServerError("Could not find datastream: $user/$group/$stream");

        if ($res->num_rows == 0) 
                badRequest("Invalid datastream ID: $user/$group/$stream");

        if ($res->num_rows > 1) 
                internalServerError("Duplicate datastreams?");

        $row = $res->fetch_assoc();

        if (!is_numeric($row['id'])) 
                internalServerError("Invalid datastream ID?: " . $row['id']);        

        return (int) $row['id'];
}

/*

  Accepted IDs:
  - user-id/group-id
  - user-id/group-name
  - user-name/group-id
  - user-name/group-name

*/
function get_group_id($user, $group)
{
        global $mysqli;

        $owner_is_id = is_numeric($user);
        $owner_is_name = is_valid_username($user);
        $group_is_id = is_numeric($group);
        $group_is_name = is_valid_identifier($group);

        if ($owner_is_id && $group_is_id) {
                $query = ("SELECT g.id as id from account a, `group` g "
                          . "WHERE g.owner=" . $user . " " 
                          . "AND g.id=" . $group);

        } else if ($owner_is_id && $group_is_name) {
                $query = ("SELECT g.id as id from `group` g "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "'");
                        
        } else if ($owner_is_name && $group_is_id) {
                $query = ("SELECT g.id as id from account a, `group` g "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.id=" . $group);

        } else if ($owner_is_name && $group_is_name) {
                $query = ("SELECT g.id as id from account a, `group` g "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "'");

        } else {
                badRequest("Invalid group ID: " . implode("/", $a));
        }

        //echo $query . "\n";

        $res = $mysqli->query($query);
        if (!$res) internalServerError("Could not find group: $user/$group");

        if ($res->num_rows == 0) 
                badRequest("Invalid group ID: $user/$group");

        if ($res->num_rows > 1) 
                internalServerError("Duplicate groups?");

        $row = $res->fetch_assoc();

        if (!is_numeric($row['id'])) 
                internalServerError("Invalid group ID?: " . $row['id']);        

        return (int) $row['id'];
}

// Using datastream ID
// ID.json
//
// Using username
// username/name-group/name-datastream.json

function get_datastream_selector($path)
{
        if (strlen($path) < 6) 
                badRequest("Invalid datastream selection: " . $path);

        $ext = substr($path, -5);
        if ($ext != ".json") 
                badRequest("Invalid datastream format: " . $path);

        $a = array("format" => "json");
        
        $s = substr($path, 0, -5);
        $parts = explode("/", $s);

        $num = count($parts);
        if ($num == 0) 
                badRequest("Invalid datastream selection: " . $path);

        if (($num == 1) && is_numeric($parts[0])) {
                // Using datastream ID
                $a['datastream'] = $parts[0];
        } else if ($num == 3) {
                // Using username
                $a['datastream'] = get_datastream_id($parts[0], $parts[1], $parts[2]);
        } else {
                badRequest("Invalid datastream selection: " . $path);
        }

        return $a;
}

// Using photostream ID
// ID.json
//
// Using username
// username/name-group/name-photostream.json

function get_photostream_selector($path)
{
        if (strlen($path) < 6) 
                badRequest("Invalid photostream selection: " . $path);

        $ext = substr($path, -5);
        if ($ext != ".json") 
                badRequest("Invalid photostream format: " . $path);

        $a = array("format" => "json");
        
        $s = substr($path, 0, -5);
        $parts = explode("/", $s);

        $num = count($parts);
        if ($num == 0) badRequest("Invalid photostream selection: " . $path);

        if (is_numeric($parts[0])) {
                // Using photostream ID
                $a['photostream'] = $parts[0];
        } else {
                // Using username
                if ($num < 3) badRequest("Invalid photostream selection: " . $path);
                $a['photostream'] = get_photostream_id($parts[0], $parts[1], $parts[2]);
        }

        return $a;
}

/*

  Accepted IDs:
  - user-id/group-id/stream-index
  - user-id/group-id/stream-name
  - user-id/group-name/stream-index
  - user-id/group-name/stream-name
  - user-name/group-id/stream-index
  - user-name/group-id/stream-name
  - user-name/group-name/stream-index
  - user-name/group-name/stream-name

*/
function get_photostream_id($user, $group, $stream)
{
        global $mysqli;

        $owner_is_id = is_numeric($user);
        $owner_is_name = is_valid_username($user);
        $group_is_id = is_numeric($group);
        $group_is_name = is_valid_identifier($group);
        $stream_is_index = is_numeric($stream);
        $stream_is_name = is_valid_identifier($stream);

        if ($owner_is_id && $group_is_id && $stream_is_index) {
                $query = ("SELECT gd.photostream as id from account a, `group` g, group_photostreams gd "
                          . "WHERE g.owner=" . $user . " " 
                          . "AND g.id=" . $group . " "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_id && $group_is_id && $stream_is_name) {
                $query = ("SELECT gd.photostream as id from account a, `group` g, group_photostreams gd, photostream d "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.id='" . $group . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.photostream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");

        } else if ($owner_is_id && $group_is_name && $stream_is_index) {
                $query = ("SELECT gd.photostream as id from `group` g, group_photostreams gd "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_id && $group_is_name && $stream_is_name) {
                $query = ("SELECT gd.photostream as id from `group` g, group_photostreams gd, photostream d "
                          . "WHERE g.owner=" . $user . " "
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.photostream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");
                        
        } else if ($owner_is_name && $group_is_id && $stream_is_index) {
                $query = ("SELECT gd.photostream as id from account a, `group` g, group_photostreams gd "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.id='" . $group . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_name && $group_is_id && $stream_is_name) {
                $query = ("SELECT gd.photostream as id from account a, `group` g, group_photostreams gd, photostream d "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.id=" . $group . " "
                          . "AND g.id=gd.gid "
                          . "AND gd.photostream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");

        } else if ($owner_is_name && $group_is_name && $stream_is_index) {
                $query = ("SELECT gd.photostream as id from account a, `group` g, group_photostreams gd "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.index=" . $stream);

        } else if ($owner_is_name && $group_is_name && $stream_is_name) {
                $query = ("SELECT gd.photostream as id from account a, `group` g, group_photostreams gd, photostream d "
                          . "WHERE a.username='" . $mysqli->real_escape_string($user) . "' " 
                          . "AND a.id=g.owner " 
                          . "AND g.name='" . $mysqli->real_escape_string($group) . "' "
                          . "AND g.id=gd.gid "
                          . "AND gd.photostream=d.id " 
                          . "AND d.name='" . $mysqli->real_escape_string($stream) . "'");

        } else {
                badRequest("Invalid photostream ID: " . implode("/", $a));
        }

        //echo $query . "\n";

        $res = $mysqli->query($query);
        if (!$res) internalServerError("Could not find photostream: $user/$group/$stream");

        if ($res->num_rows == 0) 
                badRequest("Invalid photostream ID: $user/$group/$stream");

        if ($res->num_rows > 1) 
                internalServerError("Duplicate photostreams?");

        $row = $res->fetch_assoc();

        if (!is_numeric($row['id'])) 
                internalServerError("Invalid photostream ID?: " . $row['id']);        

        return (int) $row['id'];
}


// Using datastream ID
// ID.json
//
// Using name
// name.json

function get_app_selector($path)
{
        if (strlen($path) < 6) 
                badRequest("Invalid app selection: " . $path);

        $ext = substr($path, -5);
        if ($ext != ".json") 
                badRequest("Invalid app format: " . $path);

        $a = array("format" => "json");
        
        $s = substr($path, 0, -5);

        if (is_numeric($s)) {
                // Using app ID
                $a['id'] = (int) $s;
        } else if (is_valid_identifier($s)) {
                // Using username
                $a['name'] = $s;
        } else {
                badRequest("Invalid app selection: " . $path);
        }

        return $a;
}


?>
