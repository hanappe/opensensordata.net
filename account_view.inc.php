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
?>
<?php include "page_header.inc.php"; ?>
     <div class="account_info">
       <div class="account_username">Username: <?php echo $selected_account->username ?></div>

     </div>

<script>
</script>
         
     <div class="account_groups">
       
       <h2>Groups</h2>
       
<?php

   for ($i = 0; $i < count($groups); $i++) {
        $d = $groups[$i];
        echo $d->name . "<br/>";             
  }

?>

     </div>

     <div class="account_datastreams">       
       <h2>Datastreams</h2>
       
       <table>
         <tr>
           <th>Id</th>
           <th>Name</th>
           <th>Description</th>
           <th>Timezone</th>
           <th>Longitude</th>
           <th>Latitude</th>
           <th>Unit</th>
         </tr>
<?php
   for ($i = 0; $i < count($datastreams); $i++):
        $d = $datastreams[$i];
        $idshort = "datastream_" . $d->id . "_short";
        $idlong = "datastream_" . $d->id;
?>
         <tr>
           <td class="enumerate left"><?php echo $d->id ?></td>
           <td class="enumerate left"><a href="#" onclick="edit()"><?php echo $d->name ?></a></td>
           <td class="enumerate left"><a href="#" onclick="edit()"><?php echo $d->description ?></a></td>
           <td class="enumerate center"><a href="#" onclick="edit()"><?php if ($d->timezone) echo $d->timezone; else echo '-' ?></a></td>
           <td class="enumerate center"><a href="#" onclick="edit()"><?php if ($d->longitude) echo $d->longitude; else echo '-' ?></a></td>
           <td class="enumerate center"><a href="#" onclick="edit()"><?php if ($d->latitude) echo $d->latitude; else echo '-' ?></a></td>
           <td class="enumerate center"><a href="#" onclick="edit()"><?php if ($d->unit) echo $d->unit; else echo '-' ?></a></td>
         </tr>
<?php endfor; ?>
       </table>
     </div>

<?php include "page_footer.inc.php"; ?>
