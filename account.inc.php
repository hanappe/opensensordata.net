<?php /* 

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
require_once "account_stats.inc.php";

$account = NULL;

function load_account()
{
        global $account;

        session_start();

        $op = $_REQUEST['op'];
        if ($op == "logout") {
                unset($_SESSION['account']);
                unset($_SESSION['timeout']);
                return;
        }

        $key = input_get_key();
        $username = input_get_username();

        if (isset($_SESSION['account'])) {
                if (!isset($_SESSION['timeout'])) {
                        unset($_SESSION['account']);
                } else if ($_SESSION['timeout'] + 3600 < time()) {
                        unset($_SESSION['account']);
                } else {
                        $account = $_SESSION['account'];
                        $_SESSION['timeout'] = time();
                }

        } else if (is_valid_key($key)) {

                db_connect();

                $account = new Account();
                if (!$account->load("key", $key)) 
                        badRequest("Invalid key");
                if (!$account->validated) 
                        unauthorized("Invalid account");

                db_close();

                $_SESSION['account'] = $account;
                $_SESSION['timeout'] = time();

        } else if (is_valid_username($username)) {

                $pwhash = input_get_pwhash();
                if (!is_valid_pwhash($pwhash)) {
                        return;
                }

                db_connect();

                $account = new Account();
                if (!$account->load("username", $username)) {
                        db_close();
                        $account = NULL;
                        return;
                }

                db_close();

                if (!$account->validated) 
                        unauthorized("Invalid account");

                if ($account->pwhash != $pwhash) {
                        $account = NULL;
                        return;
                }

                $_SESSION['account'] = $account;
                $_SESSION['timeout'] = time();
        }
}

class Account
{
        public $id;
        public $username;
        public $email;
        public $pwhash;
        public $key;
        public $validated;
        public $code;
        public $err;

        public function load($field, $value) 
        {
                global $mysqli;

                $query = ("SELECT `id`, `username`, `email`, "
                          . "`pwhash`, `key`, `validated`, `code` FROM account WHERE " 
                          . "`" . $field . "`='" . $mysqli->real_escape_string($value) . "'");

                //echo $query . "<br/>\n";

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) return false;
                $row = $res->fetch_assoc();

                $this->id = $row['id'];
                $this->username = $row['username'];
                $this->email = $row['email'];
                $this->pwhash = $row['pwhash'];
                $this->key = $row['key'];
                $this->validated = $row['validated'];
                $this->code = $row['code'];

                return true;
        }

        public function generateUUID()
        {
                // Generate an UUID, sort of...
                return sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
                               // 32 bits for "time_low"
                               mt_rand(0, 0xffff), mt_rand(0, 0xffff),
                                     
                               // 16 bits for "time_mid"
                               mt_rand(0, 0xffff),
                                     
                               // 16 bits for "time_hi_and_version",
                               // four most significant bits holds version number 4
                               mt_rand(0, 0x0fff) | 0x4000,
                       
                               // 16 bits, 8 bits for "clk_seq_hi_res",
                               // 8 bits for "clk_seq_low",
                               // two most significant bits holds zero and one for variant DCE1.1
                               mt_rand(0, 0x3fff) | 0x8000,
                       
                               // 48 bits for "node"
                               mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff));
        }

        public function generateKey()
        {
                $this->key = $this->generateUUID();
        }

        public function generateCode()
        {
                $this->code = $this->generateUUID();
        }
        
        public function create() 
        {
                global $mysqli;
                
                if (isset($this->id)) 
                        return false;

                $query = ("INSERT INTO account VALUES ("
                          . "NULL,"
                          . "'" . $mysqli->real_escape_string($this->username) . "',"
                          . "'" . $mysqli->real_escape_string($this->email) . "',"
                          . "'" . $mysqli->real_escape_string($this->pwhash) . "',"
                          . "'" . $mysqli->real_escape_string($this->key) . "',"
                          . $mysqli->real_escape_string($this->validated) . ","
                          . "'" . $mysqli->real_escape_string($this->code) . "')");

                //echo $query . "<br/>\n";

                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        $this->err = $mysqli->error;
                        return false;
                }

                $this->id = $mysqli->insert_id;

                $stats = new AccountStats();
                $stats->data_maxuploads = -1;
                $stats->photo_maxuploads = -1;
                if (!$stats->create($this->id)) {
                        $this->err = $stats->err;
                        return false;
                }

                return true;
        }

        public function validate() 
        {
                global $mysqli;
                
                if (!isset($this->id)) 
                        return false;

                $query = ("UPDATE account SET validated=1 WHERE id=" . $this->id . ";");
                //echo $query . "<br/>\n";

                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        $this->err = $mysqli->error;
                        return false;
                }

                return true;
        }

        function loadGroups()
        {
                global $mysqli;

                $groups = array();

                $query = ("SELECT * FROM `group` WHERE "
                          . "`owner`=" . $mysqli->real_escape_string($this->id) 
                          . " ORDER By name;");
                //echo $query;
                
                $res = $mysqli->query($query);
                if (!$res) {
                        //echo $mysqli->error;
                        return false;
                }
                
                while ($row = $res->fetch_assoc()) {
                        $group = new Group();
                        $group->import($row);
                        $group->loadDatastreams();
                        $group->loadPhotostreams();
                        $groups[] = $group;
                }

                return $groups;
        }

        function loadDatastreams()
        {
                global $mysqli;

                $datastreams = array();

                $query = "SELECT * FROM `datastream` WHERE `owner`=" . $mysqli->real_escape_string($this->id) . " ORDER By name;";
                //echo $query;
                
                $res = $mysqli->query($query);
                if (!$res) {
                        //echo $mysqli->error;
                        return false;
                }
                
                while ($row = $res->fetch_assoc()) {
                        $datastream = new Datastream();
                        $datastream->import($row);
                        $datastreams[] = $datastream;
                }

                return $datastreams;
        }

        function loadPhotostreams()
        {
                global $mysqli;

                $photostreams = array();

                $query = "SELECT * FROM `photostream` WHERE `owner`=" . $mysqli->real_escape_string($this->id) . " ORDER By name;";
                //echo $query;
                
                $res = $mysqli->query($query);
                if (!$res) {
                        //echo $mysqli->error;
                        return false;
                }
                
                while ($row = $res->fetch_assoc()) {
                        $photostream = new Photostream();
                        $photostream->import($row);
                        $photostreams[] = $photostream;
                }

                return $photostreams;
        }
}

?>
