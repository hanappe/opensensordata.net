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
?><html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
  <head>
    <title>OpenSensorData.net - Create account</title>
    <link rel="stylesheet" type="text/css" href="nostyle.css" />

<script type="text/javascript"> 

var validForm;
var errorstring;
var firstError;

function submitForm() 
{
    if (validate()) {
        document.forms['input'].submit();
    }
}

function validate() 
{
    validForm = true;
    firstError = null;
    errorstring = '';
    var username = document.forms['input'].username.value;
    var email = document.forms['input'].email.value;
    var pw = document.forms['input'].pw.value;
    var pwcheck = document.forms['input'].pwcheck.value;
    var agree = document.forms['input'].agree.checked;
    var elements = document.forms['input'].elements;
    for (var i = 0; i < elements.length; i++) {
	if (!elements[i].value || (elements[i].value == '')) {
	    writeError(elements[i], 'This field is required: ');
        }
    }
    if (username.length < 3)
	writeError(elements['username'], 'The username is too short.');
    if (username.length > 24)
	writeError(elements['username'], 'The username is too long.');
    if (pw.length < 8)
	writeError(elements['pw'], 'The password is too short.');
    if (pw.length > 256)
	writeError(elements['pw'], 'The password is too long.');
    if (pw != pwcheck)
	writeError(elements['pwcheck'], 'Please verify the password');
    if (email.indexOf('@') == -1) // FIXME!!!
	writeError(elements['email'], 'This is not a valid email address');
    if (email.indexOf('.') == -1) // FIXME!!!
	writeError(elements['email'], 'This is not a valid email address');
    if (!agree)
	writeError(elements['agree'], 'You must agree with the license to create an account');
    if (!document.getElementById)
	alert(errorstring);
    if (firstError) 
	firstError.focus();
    return validForm;
}

function writeError(obj, message) 
{
    validForm = false;
    if (obj.hasError) return;
    obj.hasError = true;
    if (document.getElementById) {
        var td = document.getElementById(obj.name + "_msg");
        if (td) {
	    td.oldText = td.innerHTML;
            td.innerHTML = message;
	    obj.onchange = removeError;
        }

    } else {
	errorstring += obj.name + ': ' + message + '\n';
    }
    if (!firstError)
	firstError = obj;
}

function removeError()
{
    if (document.getElementById) {
        var td = document.getElementById(this.name + "_msg");
        if (td) {
            td.innerHTML = td.oldText;
            td.className = "info";
        }
    }
    this.hasError = false;
    this.onchange = null;
}
</script>

  </head>
  <body>

    <div class="account">

      <form action="createaccount.php" method="post" name="input" onsubmit="return validate();">
        <input type="hidden" name="op" value="create" />
        <table>
          <tr>
            <td colspan="3"><?php echo $message; ?></td>
          </tr>
          <tr>
            <td class="label"><label for="username">User name</label></td>
            <td class="input"><input type="text" name="username" size="20" maxlength="32" value="<?php echo $username ?>" /></td>
            <td class="message">
              <span class="warning" id="name_msg"></span>
              <span class="info" id="name_info">Your username. This name should be unique and should be at least 8 characters long. Spaces and special characters are not allowed.</span>
            </td>
          </tr>
          <tr>
            <td class="label"><label for="email">Email</label></td>
            <td class="input"><input type="text" name="email" size="20" maxlength="100" value="<?php echo $email ?>" /></td>
            <td class="message">
              <span class="warning" id="email_msg"></span>
              <span class="info" id="email_info">Your email address.</span>
            </td>
          </tr>
          <tr>
            <td class="label"><label for="pw">Password</label></td>
            <td class="input"><input type="password" name="pw" size="20" maxlength="30" value="<?php echo $pw ?>" /></td>
            <td class="message">
              <span class="warning" id="pw_msg"></span>
              <span class="info" id="pw_info">Your password. It should be at least 8 characters long and contain special characters or digits.</span>
            </td>
          </tr>
          <tr>
            <td class="label"><label for="pwcheck">Check password</label></td>
            <td class="input"><input type="password" name="pwcheck" size="20" maxlength="30" value="<?php echo $pw ?>" /></td>
            <td class="message">
              <span class="warning" id="pwcheck_msg"></span>
              <span class="info" id="pwcheck_info"></span>
            </td>
            <td class="message"></td>
          </tr>
          <tr>
            <td class="label"></td>
            <td class="input">
              <img id="captcha" src="/securimage/securimage_show.php" alt="CAPTCHA Image" /><br/>
	      <input type="text" name="captcha_code" size="10" maxlength="6" />
              [<a href="#" onclick="document.getElementById('captcha').src = '/securimage/securimage_show.php?' + Math.random(); return false">Different Image</a>]
            </td>
            <td class="message"></td>
          </tr>
          
          <tr>
            <td class="label"></td>
            <td class="license">
              The OpenSensorData.net database is made available under
              Open Database License whose full text can be found at
              <a href="http://opendatacommons.org/licenses/odbl/">http://opendatacommons.org/licenses/odbl/</a>. Any
              rights in
              individual contents of the database are licensed under the Database
              Contents License whose text can be found
              <a href="http://opendatacommons.org/licenses/dbcl/">http://opendatacommons.org/licenses/dbcl/</a>.
            </td>
            <td class="message"></td>
          </tr>
          
          <tr>
            <td class="label"></td>
            <td class="input"><input type="checkbox" name="agree" value="yes" <?php if ($agree == "yes") echo "checked" ?> />I accept the Open Database License</td>
            <td class="message"><span class="warning" id="agree_msg"></span>
            </td>
              
          </tr>
          
          <tr>
            <td class="label"></td>
            <td class="input">[<a href="#" onclick="submitForm();">Create</a>]</td>
            <td class="message"></td>
          </tr>

        </table>
      </form>
    </div>
  </body>
</html>
