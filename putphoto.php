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
require_once "account_stats.inc.php";
require_once "photostream.inc.php";

db_connect();

$key = input_get_key();
if (!is_valid_key($key)) 
        badRequest("Invalid key");

$account = new Account();
if (!$account->load("key", $key)) 
        badRequest("Invalid key");
if (!$account->validated) 
        unauthorized("Invalid account");

$path = $_GET['path'];

$selector = new Selector();
if (!$selector->parse_photo($path))
        badRequest($selector->err);

$photostream = new Photostream();
if (!$photostream->load($selector->id))
        badRequest("Invalid photostream.");

if ($photostream->owner != $account->id) 
        unauthorized("");

$tmpname = tempnam($tmpphotodir, "osd");
$fp = fopen($tmpname, "w");
if (!$fp) {
        unlink($tmpname);
        internalServerError("Failed to store the photo (1).");
}
//fwrite($fp, file_get_contents('php://input'));
$data = file_get_contents('php://input');
$image = base64_decode($data);
fwrite($fp, $image);
fclose($fp);

// Basic sanity checks

if (filesize($tmpname) > 2000000) { 
        unlink($tmpname);
        badRequest("File too big (max. 2MB).");
}

$finfo = finfo_open(FILEINFO_MIME_TYPE);
$mimetype = finfo_file($finfo, $tmpname);
if ($mimetype != "image/jpeg") {
        //unlink($tmpname);
        badRequest("Only JPEG images are supported ($tmpname : $mimetype).");
}

$size = getimagesize($tmpname);
if ($size[0] < 100) badRequest("Image width too small.");
if ($size[0] > 4000) badRequest("Image too wide.");
if ($size[1] < 75) badRequest("Image height too small.");
if ($size[1] > 3000) badRequest("Image too high.");

// Check upload limits

$stats = new AccountStats();
if (!$stats->load($account->id) && !$stats->create($account->id)) {
        internalServerError("The access to the account statistics failed.");
}

$curweek = (int) date("YW");
if ($stats->photo_curweek != $curweek) {
        // Reset the counter every week.
        $stats->photo_curweek = $curweek;
        $stats->photo_uploads = 0;
 }
if (($stats->photo_maxuploads >= 0) 
    && ($stats->photo_uploads > $stats->photo_maxuploads)) { 
        badRequest("Too many uploads.");
 }
$stats->photo_uploads++;
$stats->update();

// Copy the image

$filename = Photostream::to_path($photodir, $selector, "orig");

$dir = dirname($filename);
if (!is_dir($dir) && !mkdir($dir, 0775, true)) {
        unlink($tmpname);
        internalServerError("Failed to store the photo (2).");
}

if (!rename($tmpname, $filename)) {
        unlink($tmpname);
        internalServerError("Failed to store the photo (3).");
}
if (!chmod($filename, 0644)) {
        unlink($tmpname);
        internalServerError("Failed to store the photo (4).");
}

// Make sure the temp file is removed.
if (file_exists($tmpname))
        unlink($tmpname);

// Generate the scaled versions of the image

$img = new Imagick($filename);

$img->scaleImage(640, 0); 
$d = $img->getImageGeometry();
if ($d['height'] > 640)  $img->scaleImage(0, 640); 
$filename = Photostream::to_path($photodir, $selector, "medium");
$img->writeImage($filename);


$img->resizeImage(240, 180, Imagick::FILTER_LANCZOS, 1);
$filename = Photostream::to_path($photodir, $selector, "small");
$img->writeImage($filename);


$img->resizeImage(100, 75, Imagick::FILTER_LANCZOS, 1);
$filename = Photostream::to_path($photodir, $selector, "thumb");
$img->writeImage($filename);

$img->destroy();

// Store data in database

$query = ("REPLACE INTO photos VALUES ("
          . $selector->id . "," 
          . $selector->date->year . "," 
          . $selector->date->month . "," 
          . $selector->date->day . "," 
          . $selector->date->hour . "," 
          . $selector->date->minute . "," 
          . $selector->date->second . "," 
          . "'" . $selector->date->format("Y-m-d H:M:S") . "'," 
          . "NULL," 
          . "NULL," 
          . "NULL," 
          . "'',"
          . "NULL);" );
//echo "$query\n";

$mysqli->query($query);
if ($mysqli->errno) {
        internalServerError("Failed to store the photo (6): " . $mysqli->error);
} 

db_close();

?>
