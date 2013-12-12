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

// example: downForMaintenanceUntil("Sat, 8 Oct 2011 18:27:00 GMT");
function downForMaintenanceUntil($date)
{
        header('HTTP/1.1 503 Service Temporarily Unavailable');
        header('Retry-After: $date');
        exit(0);
}

function error($n, $s, $msg)
{
        header("HTTP/1.1 $n $s");
        header("Content-type: text/plain");
        if ($msg) 
                echo "{ \"error\": true, \"msg\": \"" . $msg . "\" }\n";
        else 
                echo "{ \"error\": true, \"msg\": \"" . $s . "\" }\n";
        db_close(); // FIXME
        exit();
}

function badRequest($msg)
{
        error(400, 'Bad Request', $msg);
}

function notFound()
{
        error(404, 'Not found', "");
}

function internalServerError($msg)
{
        error(500, 'Internal Server Error', $msg);
}

function unauthorized($msg)
{
        error(401, 'Unauthorized', $msg);
}

?>
