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

load_account();

?><html>
<html>
  <head>
    <title>OpenSensorData.net</title>
    <style>
body {
      background: white;
      width: 220px;
      height: 60px;
      font-family: "Helvetica Neue", Helvetica, Arial, "Lucida Grande", sans-serif;
      font-weight: normal; 
      font-size: 10px;
}
td.label { font-size: 10px; }
td.input { font-size: 10px; }
input[type=text] { font-size: 10px; }
input[type=password] { font-size: 10px; }
a:link { color: #000000; text-decoration: none; font-weight: bold; }
a:visited { color: #000000; text-decoration: none; font-weight: bold; }
a:active { color: #000000; text-decoration: none; font-weight: bold; }
a:hover { color: #afafaf; text-decoration: none; font-weight: bold; }
    </style>
    <script src="/md5.js" type="text/javascript"></script>
  </head>

  <body>
<?php if (isset($account)):  ?>
    <div>
        Hello <?php echo $account->username; ?>.<br/>
        <a href="/accounts/<?php echo $account->username; ?>" target="_parent">account</a> |
        <a href="account_panel.php?op=logout">logout</a>
    </div>
<?php else:  ?>
<script type="text/javascript"> 
function submitForm() 
{
        var username = document.forms['input'].username.value;
        var pw = document.forms['input'].pwhash.value;
        document.forms['input'].pwhash.value = hex_md5(username + pw);
        document.forms['input'].submit();
}
</script>

    <div>
      <form action="account_panel.php" method="post" name="input" onsubmit="return submitForm();">
        <table>
          <tr>
            <td class="label">Username</td>
            <td class="input"><input type="text" name="username" size="8" maxlength="32" value="" /></td>
          </tr>
          <tr>
            <td class="label">Password</td>
            <td class="input">
              <input type="password" name="pwhash" size="8" maxlength="30" value="" />
            </td>
          </tr>
        </table>
        <input type="submit" style="display:none"/>
      </form>
    </div>
<?php endif;  ?>
  </body>
</html>
