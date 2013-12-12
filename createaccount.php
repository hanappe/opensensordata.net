<?
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

session_start();

require_once "input.inc.php";
require_once "db.inc.php";
require_once "error.inc.php";
require_once "account.inc.php";
require_once "securimage/securimage.php";
require_once "app.inc.php";

$username = '';
$email = '';
$pw = '';
$agree = 'no';
$message = '';

$op = isset($_REQUEST['op'])? $_REQUEST['op'] : NULL;

if ($op == NULL) {
        require_once "createaccount.form.inc.php";
        exit(0);
}

if ($op == 'create') {
        $username = input_get_username();
        $email = input_get_email();
        $pw = input_get_password();
        $agree = $_POST['agree'];

        if ($agree != 'yes') {
                $message = "You must accept the Open Database License to create an account on OpenSensorData.net";
                require_once "createaccount.form.inc.php";
                exit(0);
        }
        if (!is_valid_username($username)) {
                $message = "Please enter a valid account name (min. 3 characters, starts with a letter, digits accepted): " . $username;
                require_once "createaccount.form.inc.php";
                exit(0);
        }
        if (!is_valid_email($email)) {
                $message = "Please verify your email address. It failed to validate.";
                require_once "createaccount.form.inc.php";
                exit(0);
        }
        if (!is_valid_password($pw)) {
                $message = "Please verify your password. It failed to validate.";
                require_once "createaccount.form.inc.php";
                exit(0);
        }

        $securimage = new Securimage();
        if ($securimage->check($_POST['captcha_code']) == false) {
                $message = "The security code entered was incorrect. Please try again.";
                require_once "createaccount.form.inc.php";
                exit(0);
        }

        db_connect();

        $account = new Account();

        // Check for an existing account
        if ($account->load("username", $username)) {
                $message = "The username you entered is already used. Please select another one. Sorry...";
                require_once "createaccount.form.inc.php";
                db_close();
                exit(0);
        }

        $account->username = $username;
        $account->email = $email;
        $account->pwhash = md5($username . $pw);
        $account->generateKey();
        $account->validated = 0;
        $account->generateCode();

        if (!$account->create()) 
                internalServerError("Create: Database error");


        $app = new App();
        if ($app->load("id", 1)) {
                $app->linkAccount($account);
        }


        $subject = 'Welcome to OpenSensorData.net!';
        $message = 
"Welcome to OpenSensorData.net!

To validate your account, please open the following link: 
" . $base_url . "/createaccount.php?op=validate&code=" . $account->code . "

Cheers!
";

        $headers = ('From: ' . $contact_address . "\r\n" .
                    'Reply-To: ' . $contact_address);

        mail($account->email, $subject, $message, $headers);


        $subject = '[OpenSensorData.net] New account';
        $message = "email " . $account->email . ", username '" . $account->username . "'";
        $headers = ('From: ' . $contact_address . "\r\n");
        mail($contact_address, $subject, $message, $headers);


        require_once "createaccount.mail.inc.php";

        db_close();

} else if ($op == 'validate') {

        $code = $_GET['code'];
        if (!is_valid_key($code))
                badRequest("Invalid code " . $code);

        db_connect();

        $account = new Account();
        if (!$account->load("code", $code)) 
                internalServerError("Failed to find the account associated with the code " 
                                    . $code);
        if (!$account->validate()) 
                internalServerError("Validate: Database error");

        $subject = 'Your accounnt at OpenSensorData.net!';
        $message = 
"Thank you for joining OpenSensorData.net!

Your account is now created and validated. To create datastreams 
and upload data, you will have to use your account key:

    " . $account->key . "

Check the documentation on OpenSensorData.net for more details.

Cheers!
";

        $headers = ('From: ' . $contact_address . "\r\n" .
                    'Reply-To: ' . $contact_address);

        mail($account->email, $subject, $message, $headers);

        $subject = '[OpenSensorData.net] Account validated';
        $message = "email " . $account->email . ", username '" . $account->username . "'";
        $headers = ('From: ' . $contact_address . "\r\n");
        mail($contact_address, $subject, $message, $headers);
        
        require_once "createaccount.result.inc.php";

        db_close();
}

?>
