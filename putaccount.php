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
require_once "app.inc.php";

db_connect();

$key = input_get_appkey();
if (!is_valid_key($key)) 
        badRequest("Invalid key");

$app = new App();
if (!$app->load("key", $key)) 
        badRequest("Invalid key");

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

function get_random_string($length)
{
        $valid_chars = "abcdefghijklmnopqrstuvwABCDEFGHIJKLMNOPQRSTUVW0123456789@:.?!%&*-_=+<>#~";

        $random_string = "";
        $num_valid_chars = strlen($valid_chars);
        
        for ($i = 0; $i < $length; $i++) {
                $random_pick = mt_rand(1, $num_valid_chars);
                $random_char = $valid_chars[$random_pick-1];
                $random_string .= $random_char;
        }

        return $random_string;
}

$pw = get_random_string(8);

$account = new Account();

if ($account->load("username", $input->username)) 
        badRequest("Duplicate username");

if (!is_valid_username($input->username))
        badRequest("Invalid username");

if (!is_valid_email($input->email))
        badRequest("Invalid email");

$account->username = $input->username;
$account->pwhash = md5($input->username . $pw);
$account->email = $input->email;
$account->generateKey();
$account->validated = 1;
$account->generateCode();

if (!$account->create()) 
        internalServerError("Create: Database error");


$app->linkAccount($account); 


db_close();

unset($account->pwhash);
unset($account->validated);
unset($account->code);

header('Content-type: application/json');
echo json_encode($account) . "\n";

?>
