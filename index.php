<!DOCTYPE html>
<html>
  <head>
    <title>OpenSensorData.net</title>
    <link rel="stylesheet" type="text/css" href="opensensordata.css"> 
  </head>
  <body>
    <div class="screen">

   <div class="maketitle">

<h2 class="titleHead">OpenSensorData.net</h2>
<div class="author" ></div><br />
<div class="date" ></div>
   </div>
<p>   OpenSensorData.net is a hosting service where you can store streams of data
that come from any sensor or sensor networks. The use of OpenSensorData.net is
free (like in <span 
class="cmti-12">free beer</span>). And the software is Free (like in <span 
class="cmti-12">freedom</span>), under the
<a 
href="http://www.gnu.org/licenses/gpl.html" >GPLv3</a>. You can <a 
href="https://github.com/hanappe/opensensordata.net" >download the code</a> and install it on your own servers
(requirements: Apache/PHP/MySQL).
   </p> 

   <h3 class="likesectionHead"><a 
 id="x1-1000"></a>Storing your data on OpenSensorData.net</h3>
<p>
      </p> 
<ol  class="enumerate1" >
      <li 
  class="enumerate" id="x1-1002x1">First you must <a 
href="#x1-2000doc">know your rights</a>.
      </li>
      <li 
  class="enumerate" id="x1-1004x2">Then        you        must        create        an        account        at
      <a 
href="http://opensensordata.net/createaccount.php" class="url" ><span 
class="cmtt-12">http://opensensordata.net/createaccount.php</span></a>
      </li>
      <li 
  class="enumerate" id="x1-1006x3">The account creation will generate an API key. Write it down. You will
      need it to access the services.
                                                                          

                                                                          
      </li>
      <li 
  class="enumerate" id="x1-1008x4">Make sure you understand <a 
href="#x1-3000doc">the basic concepts</a>.
      </li>
      <li 
  class="enumerate" id="x1-1010x5"><a 
href="#x1-4000doc">Define your datastreams and groups of datastreams</a>.
      </li>
      <li 
  class="enumerate" id="x1-1012x6"><a 
href="#x1-6000doc">Upload your data</a>.
      </li>
      <li 
  class="enumerate" id="x1-1014x7"><a 
href="#x1-9000doc">Download your data</a>.
      </li>
      <li 
  class="enumerate" id="x1-1016x8"><a 
href="#x1-10000doc">View your data</a>.
      </li></ol>
<p>
   </p> 

   <h4 class="likesubsectionHead"><a 
 id="x1-2000"></a>Know your rights</h4>
<a 
 id="x1-2000doc"></a>
<p>The first thing that is important to know is that we store all the data that
you upload to OpenSensorData.net (that&#8217;s what it&#8217;s for) AND that we
make all of this data freely available to any one under the Open Database
License. The full text of the Open Database License can be found at
<a 
href="http://opendatacommons.org/licenses/odbl/" class="url" ><span 
class="cmtt-12">http://opendatacommons.org/licenses/odbl/</span></a>. Any rights in individual
contents of the database are licensed under the Database Contents License whose
text can be found <a 
href="http://opendatacommons.org/licenses/dbcl/" class="url" ><span 
class="cmtt-12">http://opendatacommons.org/licenses/dbcl/</span></a>. If you find
it difficult to comply with this, then you should set up your own data
servers.
</p> 
<p>   The second important thing is that at any time you can ask us to remove ALL
of the data associated with your account. We will make it possible for you to do
this yourself in the future. For now, however, you have to send your request by
email to the following address: someone&#x00A0;AT&#x00A0;opensensordata.net.
</p> 
<p>   The source code of OpenSensorData.net is available under <a 
href="http://www.gnu.org/licenses/gpl.html" >version 3 of the
GNU General Public License</a>. That means you can use, study, and modify it, and
that you redistribute modified copies of the source code. The lastest version of the
code is available <a 
href="latest.tar.gz" >here</a>.
                                                                          

                                                                          
</p> 
<p>
   </p> 

   <h4 class="likesubsectionHead"><a 
 id="x1-3000"></a>The basic concepts</h4>
<a 
 id="x1-3000doc"></a>
<p>A <b><span 
class="cmbx-12">datapoint</span></b> consists of the time at which the measurement took place and its
output value. For example, on Sunday, March 3rd 2013, at 11.55 am, the
temperature sensor at my home measured 8.63 degrees celsius. The data point
consists of the date (2013-03-10 11:55:00, for the precise format of the date, see
details below), and its value (8.63).
</p> 
<p>   A <b><span 
class="cmbx-12">datastream</span></b> is a series of datapoints. To add a datapoint to a datastream,
you must first create the datastream. The minimum information you have to
specify is a name, but a longer discription is welcome to. In addition, you can add
information such as the geographical location of the sensor, and the timezone in
which it is situated. All datastreams have a number that uniquely identifies it. To
add a datapoint you must specify the datastream in which you want to insert it.
You can pass along the datastream id, but there are other ways too (see <a 
href="#x1-6000doc">&#8220;Upload
your data&#8221;</a>).
</p> 
<p>   A <b><span 
class="cmbx-12">group</span></b> is a set of datastreams that you would like to keep together. Nearly
all sensor boxes or data loggers will generate several datastreams and it obvious
that you want to work with these datastreams together. Groups are flexible and it
is also possible to create a group to bundle datastreams from different locations
and from different users.
</p> 
<p>   The creation of datastreams and groups is handled in the <a 
href="#x1-4000doc">next section</a>.
</p> 
<p>
   </p> 

   <h4 class="likesubsectionHead"><a 
 id="x1-4000"></a>Define your datastreams and groups of datastreams</h4>
<a 
 id="x1-4000doc"></a>
<p>In most cases, you will not have to create groups and datastreams yourself. If the
sensor box that user OpenSensorData is well designed, you should only provide
the API key and a group name, and the device or software will create the group
and its datastreams automagically.
</p> 
<p>   In the short future, we will provide a online editor to manipulate groups and
datastreams, however.
                                                                          

                                                                          
</p> 
<p>   To create a groups and its datastreams, you must send a JSON description to
the server over HTTP. Note that you must always specify the API kay in the
X-OpenSensorData-Key header. The group definition contains a name and a
description. A name can only be used once per account to label a group. So if I
have a sensor box and it uses group name &#8217;greenhouse&#8217;, if I install a second
greenhouse I will have to choose another name.
</p> 
<p>   The set of datagroups are coded in a JSON array, and for each you must
provide its name and description. The datagroups will be automatically created
for you.
</p> 
<p>   The following example shows the complete HTTP request, with the
headers, following by and empty line, and then the JSON description in the
body.
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">PUT</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/groups/</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;application/json</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"greenhouse",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"The</span><span 
class="cmtt-12">&#x00A0;sensorbox</span><span 
class="cmtt-12">&#x00A0;of</span><span 
class="cmtt-12">&#x00A0;the</span><span 
class="cmtt-12">&#x00A0;greenhouse</span><span 
class="cmtt-12">&#x00A0;on</span><span 
class="cmtt-12">&#x00A0;my</span><span 
class="cmtt-12">&#x00A0;terrace",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"datastreams":</span><span 
class="cmtt-12">&#x00A0;[</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"temperature",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Temperature"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;},</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"Humidity",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Humidity"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;}</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;]</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
                                                                          

                                                                          
</p> 
<p>   A simple way to create the group, is to use telnet, connect to opensensordata.net
on port 80, and then copy/paste the example above.
</p> 
<p>   It is possible to add geolocation data to the datastream description. This
allows the datastream to be positioned on a map. two extra fields are used, the
longitude and latitude. You can specify a timezone. This can be useful for devices
without a real-time clock. Lastly, you can specify a the unit in which the value is
measured. For a complete list of currently supported units, see <a 
href="#units" >the list at the end
of the documentation</a>.
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;...</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"datastreams":</span><span 
class="cmtt-12">&#x00A0;[</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"temperature",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Temperature"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"longitude":</span><span 
class="cmtt-12">&#x00A0;"2.363471",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"latitude":</span><span 
class="cmtt-12">&#x00A0;"48.917536",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"timezone":</span><span 
class="cmtt-12">&#x00A0;"+1",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"unit":</span><span 
class="cmtt-12">&#x00A0;"Celsius"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;},</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;...</span>
</p> 
<p></div>
</p> 
<p>   All the error handling in OpenSensorData.net is handled through the <a 
href="http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html" >HTTP
status codes</a>. This makes it easier to communicate to the server in scripts. Status
code 200 indicates that the request was handled successfully. Pretty much
anything else will indicate an error. The body of the response will then contain a
simple plain text error message.
</p> 
<p>   If all goes well, the request above should have returned:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">HTTP/1.1</span><span 
class="cmtt-12">&#x00A0;200</span><span 
class="cmtt-12">&#x00A0;OK</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;xxx</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;text/html</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
                                                                          

                                                                          
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"id":</span><span 
class="cmtt-12">&#x00A0;"387",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"greenhouse",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"The</span><span 
class="cmtt-12">&#x00A0;sensorbox</span><span 
class="cmtt-12">&#x00A0;of</span><span 
class="cmtt-12">&#x00A0;the</span><span 
class="cmtt-12">&#x00A0;greenhouse</span><span 
class="cmtt-12">&#x00A0;on</span><span 
class="cmtt-12">&#x00A0;my</span><span 
class="cmtt-12">&#x00A0;terrace",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"datastreams":</span><span 
class="cmtt-12">&#x00A0;[</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"id":</span><span 
class="cmtt-12">&#x00A0;"9347",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"temperature",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Temperature"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;},</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"id":</span><span 
class="cmtt-12">&#x00A0;"9348",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"Humidity",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Humidity"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;}</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;]</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
</p> 
<p>   As you can see, the server returns the same JSON structure, but with an
additional ID field for the group and the new datastreams. These IDs uniquely
define the objects and can be used in other requests, to <a 
href="#x1-6000doc">upload data</a>, for
example.
</p> 
<p>   Variations of the PUT request can be used to modify the definition of an
existing group. To update a group, or datastream, you must explicitely specify the
IDs. In result of the following request, the name of the group will be changed from
&#8217;greenhouse&#8217; to &#8217;SmallGreenhouse&#8217;. Note that no definition of the datastreams is
given. As a result, no changes to the set of datastreams of the group will be made.
The server response will contain the complete description of the group in JSON,
like above.
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">PUT</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/groups/</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;application/json</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
                                                                          

                                                                          
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"id":</span><span 
class="cmtt-12">&#x00A0;"387",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"SmallGreenhouse",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
</p> 
<p>   Had we given a description of the group&#8217;s datstreams, then these would have
been updated, too. For example, you can remove all the datastreams from the
group using the request below, in which the datastreams field is explicitely set to
an empty array. The datastreams themselves will not be deleted. They will simply
be dissociated from the group.
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">PUT</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/groups/</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;application/json</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"id":</span><span 
class="cmtt-12">&#x00A0;"387",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"greenhouse",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"The</span><span 
class="cmtt-12">&#x00A0;sensorbox</span><span 
class="cmtt-12">&#x00A0;of</span><span 
class="cmtt-12">&#x00A0;the</span><span 
class="cmtt-12">&#x00A0;greenhouse</span><span 
class="cmtt-12">&#x00A0;on</span><span 
class="cmtt-12">&#x00A0;my</span><span 
class="cmtt-12">&#x00A0;terrace",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"datastreams":</span><span 
class="cmtt-12">&#x00A0;[]</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
</p> 
<p>
   </p> 

   <h5 class="likesubsubsectionHead"><a 
 id="x1-5000"></a>Define your datastreams directly</h5>
<a 
 id="x1-5000doc"></a>
<p>It is not necessary to create a group to create datastreams. Datastreams can be
created directly using a PUT request similar to the one for groups. The URL to
which you must send your request is http://opensensordata.net/datastreams/.
                                                                          

                                                                          
Here&#8217;s an example:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">PUT</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/datastreams/</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;application/json</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"temperature-4",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Temperature",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"longitude":</span><span 
class="cmtt-12">&#x00A0;"2.363471",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"latitude":</span><span 
class="cmtt-12">&#x00A0;"48.917536",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"timezone":</span><span 
class="cmtt-12">&#x00A0;"+1"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
</p> 
<p>   The name of the datastream is unique for every user, i.e.. a user X can
only create one datastream called Y. The response of the request above
will return the JSON description of the new datastream, including its
unique, global ID number. When the datastream is created, any follow-up
PUT request will update the description of the datastream, except its
ID.
</p> 
<p>
   </p> 

   <h4 class="likesubsectionHead"><a 
 id="x1-6000"></a>Upload your data</h4>
<a 
 id="x1-6000doc"></a>
<p>You can upload a set of datapoints through the following URL:
http://opensensordata.net/upload. The upload interface is designed to accept
many datapoints that have been produced by a sensor device and that are
uploaded through software, not by humans manually.
</p> 
<p>   To upload data, you must send an HTTP POST request to
http://opensensordata.net/upload, with all the datapoints in the body of the
                                                                          

                                                                          
request. The format of the datapoints is the CSV file format (Comma Separated
Values). This is a textual file format in which each line represents one datapoint.
Each line should contain at least three fields: the ID of the datastream, the date,
and the sensor value. All field values of the datapoint are separated by
commas.
</p> 
<p>   The following example makes this clear. The first five lines in the example are
the <a 
href="http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol" >HTTP request headers</a>. You can pass the key of your account in the
<b><span 
class="cmbx-12">X-OpenSensorData-Key</span></b> header.
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">POST</span><span 
class="cmtt-12">&#x00A0;/opensensordata.net/upload</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;text/csv</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">1,2013-03-03T18:29:43,6.875</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">2,2013-03-03T18:29:43,6.4375</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">3,2013-03-03T18:29:43,61.054</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">4,2013-03-03T18:29:43,61.054</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">5,2013-03-03T18:29:43,70.4128</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">6,2013-03-03T18:29:43,44.8116</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">7,2013-03-03T18:29:43,1021.48</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">8,2013-03-03T18:29:43,131.768</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">1,2013-03-03T18:30:01,6.875</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">2,2013-03-03T18:30:01,6.4375</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">3,2013-03-03T18:30:01,61.054</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">4,2013-03-03T18:30:01,61.054</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">5,2013-03-03T18:30:01,70.4128</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">6,2013-03-03T18:30:01,44.8116</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">7,2013-03-03T18:30:01,1021.48</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">8,2013-03-03T18:30:01,131.768</span>
</p> 
<p></div>
</p> 
<p>   The first column identifies the datastream. It is possible to mix datapoints of
several datastreams together in the same request. The second column is the
timestamp. OpenSensorData.net uses the <a 
href="http://en.wikipedia.org/wiki/ISO_8601" >ISO_8601</a> standard to encode the date
                                                                          

                                                                          
and time. The last column is the value.
</p> 
<p>   It is also possible to add geolocation data, for those of you who move around
with the sensor device. The fourth column should contain the longitude, and the
fifth the latitude:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">1,</span><span 
class="cmtt-12">&#x00A0;2013-03-03T18:29:43,</span><span 
class="cmtt-12">&#x00A0;6.875,</span><span 
class="cmtt-12">&#x00A0;2.363471,</span><span 
class="cmtt-12">&#x00A0;48.917536</span>
</p> 
<p></div>
</p> 
<p>   The key of your account can be passed in the X-OpenSensorData-Key header.
It is also possible to pass the key as a query string. The following example uploads
a CSV file directly using the wget program:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">wget</span><span 
class="cmtt-12">&#x00A0;--post-file=data.csv</span><span 
class="cmtt-12">&#x00A0;\</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;--output-document=data.out</span><span 
class="cmtt-12">&#x00A0;\</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"http://opensensordata.net/upload?key=12345678-xxxx-yyyy-zzzz-123456789012"</span>
</p> 
<p></div>
</p> 
<p>
   </p> 

   <h5 class="likesubsubsectionHead"><a 
 id="x1-7000"></a>More on the datastream identification</h5>
<p>The unique, numerical datastream identifier is the shortest way to specify the
datastream. There is a second way to identify the datastream that is
more human-readable. You can combine the account name, the group
name, and the datastream name to identify the stream, as in the following
example:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">peter/greenhouse/temperature,</span><span 
class="cmtt-12">&#x00A0;2013-03-03T18:29:43,</span><span 
class="cmtt-12">&#x00A0;6.875,</span><span 
class="cmtt-12">&#x00A0;2.363471,</span><span 
class="cmtt-12">&#x00A0;48.917536</span>
</p> 
<p></div>
                                                                          

                                                                          
</p> 
<p>
   </p> 

   <h5 class="likesubsubsectionHead"><a 
 id="x1-8000"></a>Default geolocation and timezone</h5>
<p>If the geolocation or the timezone is not specified in a datapoint, then the
geolocation and timezone of the datastream will be used.
</p> 
<p>
   </p> 

   <h4 class="likesubsectionHead"><a 
 id="x1-9000"></a>Download your data</h4>
<a 
 id="x1-9000doc"></a>
<p>To obtain all the data of a datastream with ID [id], use the following URL:
http://opensensordata.net/datastreams/[id].csv. We mentioned above that a datastream
can also be identified using the account name and so on. This can be used here as well:
<a 
href="http://opensensordata.net/datastreams/peter/greenhouse/temperature.csv" class="url" ><span 
class="cmtt-12">http://opensensordata.net/datastreams/peter/greenhouse/temperature.csv</span></a>.
</p> 
<p>   If you want to select a specific time slice, you can extend the URL as follows:
http://opensensordata.net/datastreams/[id]/[year]/[month]/[day]/[hour]/[minute]/[second].csv.
It is not necessary to specify down to the second. Any intermediate range value
will be exepted. For example, to get the data from March 2013, you can use:
<a 
href="http://opensensordata.net/datastreams/peter/greenhouse/temperature/2013/03.csv" class="url" ><span 
class="cmtt-12">http://opensensordata.net/datastreams/peter/greenhouse/temperature/2013/03.csv</span></a>.
</p> 
<p>
   </p> 

   <h4 class="likesubsectionHead"><a 
 id="x1-10000"></a>View your data</h4>
<a 
 id="x1-10000doc"></a>
<p>If you use OpenSensorData.net for an application, you will probably have you own
visualisation tools. Your app obtains the data through the URL explained above
and displays the results in whatever way that makes sense. It is still handy to be
able to inspect the data online at OpenSensorData.net. also so other people can
view your data.
</p> 
<p>   We currently use <a 
href="http://dygraphs.com/" >dygraph</a> to make the figures. The URLs are quite similar to
the ones above: http://opensensordata.net/viewdata/[id].csv. For example,
<a 
href="http://opensensordata.net/viewdata/peter/greenhouse/temperature/2013/06.csv" class="url" ><span 
class="cmtt-12">http://opensensordata.net/viewdata/peter/greenhouse/temperature/2013/06.csv</span></a>.
</p> 
<p>   It is also possible to view all the datastreams in a group:
<a 
href="http://opensensordata.net/viewgroup/peter/greenhouse/2013/06.csv" class="url" ><span 
class="cmtt-12">http://opensensordata.net/viewgroup/peter/greenhouse/2013/06.csv</span></a>.
                                                                          

                                                                          
</p> 
<p>
   </p> 

   <h5 class="likesubsubsectionHead"><a 
 id="x1-11000"></a>Photostreams</h5>
<a 
 id="x1-11000doc"></a>
<p>Besides datastreams, you can also create photostreams to upload the
output of image sensors. Photostreams are very similar to datastreams,
except for uploading the data which must be done one photo at a time (see
below).
</p> 
<p>   The URL to define or modify a photostream description is
http://opensensordata.net/photostreams/. Here&#8217;s an example:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">PUT</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/photostreams/</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;application/json</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"webcam",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"The</span><span 
class="cmtt-12">&#x00A0;webcam</span><span 
class="cmtt-12">&#x00A0;inside</span><span 
class="cmtt-12">&#x00A0;the</span><span 
class="cmtt-12">&#x00A0;greenhouse",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"longitude":</span><span 
class="cmtt-12">&#x00A0;"2.363471",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"latitude":</span><span 
class="cmtt-12">&#x00A0;"48.917536",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"timezone":</span><span 
class="cmtt-12">&#x00A0;"+1"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
</p> 
<p>   The name of the photostream must be unique for every user, i.e.. a user X can
only create one datastream called Y. The response of the request above
will return the JSON description of the new photostream, including its
unique, global ID number. When the photostream is created, any follow-up
PUT request will update the description of the photostream, except its
ID.
</p> 
<p>   Like datastreams, photostreams can be created and modified as part of a
group. The processing of the group description, looks for the &#8217;photostreams&#8217; field,
which must be an array:
</p> 
<p>   <div class="verbatim">
                                                                          

                                                                          
<span 
class="cmtt-12">PUT</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/groups/</span><span 
class="cmtt-12">&#x00A0;HTTP/1.1</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Host:</span><span 
class="cmtt-12">&#x00A0;opensensordata.net</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;application/json</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;100000</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"greenhouse",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"The</span><span 
class="cmtt-12">&#x00A0;sensorbox</span><span 
class="cmtt-12">&#x00A0;of</span><span 
class="cmtt-12">&#x00A0;the</span><span 
class="cmtt-12">&#x00A0;greenhouse</span><span 
class="cmtt-12">&#x00A0;on</span><span 
class="cmtt-12">&#x00A0;my</span><span 
class="cmtt-12">&#x00A0;terrace",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"datastreams":</span><span 
class="cmtt-12">&#x00A0;[</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"temperature",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Temperature"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;},</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"Humidity",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"Humidity"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;}</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;],</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"photostreams":</span><span 
class="cmtt-12">&#x00A0;[</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;{</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"name":</span><span 
class="cmtt-12">&#x00A0;"webcam",</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;"description":</span><span 
class="cmtt-12">&#x00A0;"The</span><span 
class="cmtt-12">&#x00A0;webcam</span><span 
class="cmtt-12">&#x00A0;inside</span><span 
class="cmtt-12">&#x00A0;the</span><span 
class="cmtt-12">&#x00A0;greenhouse"</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;}</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;]</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">}</span>
</p> 
<p></div>
</p> 
<p>   The JSON description of a particular photostream can be obtained using the
URL http://opensensordata.net/photostreams/id.json. The ID is the unique
numerical ID of the photostream, or the combination of the username, the name
of a group, and the name of the photostream separated by slashes. For example,
http://opensensordata.net/photostreams/peter/greenhouse/webcam.json, or
http://opensensordata.net/photostreams/1.json.
</p> 
<p>   For upload a JPEG photo, use a PUT or a POST request to the address
http://opensensordata.net/photo/stream-id/timestamp.jpg. The stream-id is, like
                                                                          

                                                                          
above, the numerical ID or the combination of user, group, and stream name.
The timestamp is a concatenation of the date and time. For example,
20130511-170002.jpg. The year is coded in 4 digits, all the other fields use 2
digits.
</p> 
<p>   The body of the request should contain the image data, without any
additional information. This makes it easier for small clients to upload images. To
push images from a shell script to the server, you can use, for example, the
following command:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">wget</span><span 
class="cmtt-12">&#x00A0;--post-file=image.jpg</span><span 
class="cmtt-12">&#x00A0;\</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;--header</span><span 
class="cmtt-12">&#x00A0;"X-OpenSensorData-Key:</span><span 
class="cmtt-12">&#x00A0;12345678-xxxx-yyyy-zzzz-123456789012"</span><span 
class="cmtt-12">&#x00A0;\</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">&#x00A0;</span><span 
class="cmtt-12">&#x00A0;http://opensensordata.net/photo/1/20130524-154300.jpg</span>
</p> 
<p></div>
</p> 
<p>   Currently only JPEG photos are accepted. We have also introduced some
upload limits to reduce the exposure to software glitches or abuse of our service.
The maximum image size is limited to 2MB and the maximum number of
uploads that you can do per week per account is 100. If these limits are
too constraint for your project, you should contact us and we will relax
them.
</p> 
<p>   You can retreive an image using the same URL as in the
upload. For example, the image uploaded above is available at
http://opensensordata.net/photo/1/20130524-154300.jpg. The server also makes a
small (320x240) and a thumbnail (100x75) version of the image available. They
have the &#8220;_s&#8221; and &#8220;_t&#8221; postfix, respectively: 20130524-154300_s.jpg for the
small image and 20130524-154300_t.jpg for the thumbnail. The name
20130524-154300_o.jpg can also be used for the original image.
</p> 
<p>   The list of images in a photostream can be retreived using
http://opensensordata.net/photos/stream-id.json,txt. The first options
returns a JSON array; the second the flat text with one photo URL per
line.
</p> 
<p>   The URL can contain a specific date, similar to getting the datapoints
from a datastream. To get the list of photos from May 24th 2013, use
http://opensensordata.net/photos/1/2013/05/24.txt.
                                                                          

                                                                          
</p> 
<p>
   </p> 

   <h3 class="likesectionHead"><a 
 id="x1-12000"></a>Error handling</h3>
<p>For all requests, you should check the HTTP status code of the response. If the
server returns &#8221;HTTP/1.1 200 OK&#8221;, all went fine. Otherwise, the HTTP status
class will give you idea of what went wrong. Most likely, one of the following
three errors occured: unauthorized access, a bad request, or an internal
server error. More details about the error can be found in the body of
the response, which contains a plain text message, like in the example
below:
</p> 
<p>   <div class="verbatim">
<span 
class="cmtt-12">HTTP/1.1</span><span 
class="cmtt-12">&#x00A0;400</span><span 
class="cmtt-12">&#x00A0;Bad</span><span 
class="cmtt-12">&#x00A0;Request</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Length:</span><span 
class="cmtt-12">&#x00A0;xxx</span>
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Content-Type:</span><span 
class="cmtt-12">&#x00A0;text/plain</span>
<span 
class="cmtt-12">&#x00A0;</span><br />
<span 
class="cmtt-12">&#x00A0;</span><br /><span 
class="cmtt-12">Invalid</span><span 
class="cmtt-12">&#x00A0;datastream</span><span 
class="cmtt-12">&#x00A0;ID:</span><span 
class="cmtt-12">&#x00A0;1932</span>
</p> 
<p></div>
</p> 
<p>
   </p> 

   <h3 class="likesectionHead"><a 
 id="x1-13000"></a>Summary of the API</h3>
<p><b><span 
class="cmbx-12">Create/update datastream</span></b><br 
class="newline" />Method: PUT<br 
class="newline" />URL: http://opensensordata.net/datastreams/<br 
class="newline" />Format: JSON<br 
class="newline" />Returns: datastream description, or HTTP error code <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Upload data</span></b> <br 
class="newline" />Method: PUT <br 
class="newline" />URL: http://opensensordata.net/upload <br 
class="newline" />Format: CSV<br 
class="newline" />Returns: HTTP status code <br 
class="newline" />
                                                                          

                                                                          
</p> 
<p>   <b><span 
class="cmbx-12">Get the data of the datastream</span></b><br 
class="newline" />Method: GET <br 
class="newline" />URL: http://opensensordata.net/datastreams/[id].csv<br 
class="newline" />URL: http://opensensordata.net/datastreams/[id]/year.csv<br 
class="newline" />URL: http://opensensordata.net/datastreams/[id]/year/month/.../second.csv<br 
class="newline" />ID: the datastream&#8217;s unique numerical ID, or the combination of
username/groupname/datastreamname<br 
class="newline" />Format: CSV<br 
class="newline" />Returns: the data <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Get the datastream description</span></b><br 
class="newline" />Method: GET <br 
class="newline" />URL: http://opensensordata.net/datastreams/[id].json<br 
class="newline" />ID: datastream unique ID, or username/groupname/datastreamname<br 
class="newline" />Format: JSON<br 
class="newline" />Returns: the datastream description, or HTTP error code <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Create/update group</span></b><br 
class="newline" />Method: PUT<br 
class="newline" />URL: http://opensensordata.net/groups/<br 
class="newline" />Format: JSON<br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Get the group description</span></b><br 
class="newline" />Method: GET <br 
class="newline" />URL: http://opensensordata.net/groups/[id].json<br 
class="newline" />ID: the group&#8217;s unique numerical ID or username/groupname.<br 
class="newline" />Format: JSON<br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Create/update photostream</span></b><br 
class="newline" />Method: PUT<br 
class="newline" />URL: http://opensensordata.net/photostreams/<br 
class="newline" />Format: JSON<br 
class="newline" />Returns: The new photostream description, or HTTP error code <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Get the photostream description</span></b><br 
class="newline" />Method: GET <br 
class="newline" />URL: http://opensensordata.net/photostreams/id.json<br 
class="newline" />with id: the photostream&#8217;s unique ID, or username/group-name/photostream-name<br 
class="newline" />Format: JSON<br 
class="newline" />Returns: The photostream description, or HTTP error code <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Upload a photo</span></b> <br 
class="newline" />Method: PUT <br 
class="newline" />URL: http://opensensordata.net/photo/id/yearmonthday-hourminutesecond.jpg
<br 
class="newline" />with id: the photostream&#8217;s unique ID, or username/group-name/photostream-name<br 
class="newline" />Format: JPG<br 
class="newline" />Returns: HTTP status code <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Get a photo</span></b> <br 
class="newline" />Method: GET <br 
class="newline" />URL: (orig version): http://opensensordata.net/photo/id/yearmonthday-hourminutesecond.jpg
<br 
class="newline" />or (orig version, explicite): http://opensensordata.net/photo/id/yearmonthday-hourminutesecond&#x02D9;o.jpg
<br 
class="newline" />or (small version, 240x180): http://opensensordata.net/photo/id/yearmonthday-hourminutesecond&#x02D9;s.jpg
<br 
class="newline" />or (thumbnail version, 100x75): http://opensensordata.net/photo/id/yearmonthday-hourminutesecond&#x02D9;t.jpg
<br 
class="newline" />Format: JPG<br 
class="newline" />Returns: HTTP status code <br 
class="newline" />
</p> 
<p>   <b><span 
class="cmbx-12">Get the list of photos in a photostream</span></b><br 
class="newline" />Method: GET <br 
class="newline" />URL: http://opensensordata.net/photostreams/id.json,txt<br 
class="newline" />URL: http://opensensordata.net/photostreams/id/year.json,txt<br 
class="newline" />URL: http://opensensordata.net/photostreams/id/year/month/.../second.json,txt<br 
class="newline" />ID: the photostream&#8217;s unique numerical ID, or the combination of
username/groupname/photostreamname<br 
class="newline" />Format: JSON or text<br 
class="newline" />Returns: JSON: An array of the photo descriptions; text: one photo URL per line
<br 
class="newline" />
                                                                          

                                                                          
</p> 
<p>
   </p> 

   <h3 class="likesectionHead"><a 
 id="x1-14000"></a>The currently available units</h3>
<a 
 id="x1-14000doc"></a>
<p>The specify the unit of the datapoints, set the &#8217;unit&#8217; field in the in a datastream
using the short names of the table below.
<a 
 id="x1-14001r1"></a>   </p> 
<!--l. 652--><div class="longtable"> <table id="TBL-2" class="longtable" 
cellspacing="0" cellpadding="0"  
><colgroup id="TBL-2-1g"><col 
id="TBL-2-1"><col 
id="TBL-2-2"><col 
id="TBL-2-3"><col 
id="TBL-2-4"></colgroup>
<tr  
 style="vertical-align:baseline;" id="TBL-2-1-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-1-1"  
class="td11">Dimension</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-1-2"  
class="td11">Short name</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-1-3"  
class="td11">Long name </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-1-4"  
class="td11">Comment</td>
</tr><tr 
class="hline"><td><hr></td><td><hr></td><td><hr></td><td><hr></td></tr><tr  
 style="vertical-align:baseline;" id="TBL-2-2-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-2-1"  
class="td11">none        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-2-2"  
class="td11">none         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-2-3"  
class="td11">none          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-2-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-3-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-3-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-3-2"  
class="td11">s              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-3-3"  
class="td11">second       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-3-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-4-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-4-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-4-2"  
class="td11">ms           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-4-3"  
class="td11">millisecond  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-4-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-5-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-5-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-5-2"  
class="td11">us            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-5-3"  
class="td11">microsecond</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-5-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-6-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-6-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-6-2"  
class="td11">min          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-6-3"  
class="td11">minute       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-6-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-7-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-7-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-7-2"  
class="td11">h              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-7-3"  
class="td11">hour          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-7-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-8-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-8-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-8-2"  
class="td11">day           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-8-3"  
class="td11">day           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-8-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-9-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-9-1"  
class="td11">time        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-9-2"  
class="td11">year          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-9-3"  
class="td11">year           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-9-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-10-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-10-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-10-2"  
class="td11">m             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-10-3"  
class="td11">meter         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-10-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-11-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-11-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-11-2"  
class="td11">mm          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-11-3"  
class="td11">millimeter   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-11-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-12-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-12-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-12-2"  
class="td11">cm           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-12-3"  
class="td11">centimeter  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-12-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-13-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-13-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-13-2"  
class="td11">km           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-13-3"  
class="td11">kilometer    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-13-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-14-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-14-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-14-2"  
class="td11">in             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-14-3"  
class="td11">inch           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-14-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-15-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-15-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-15-2"  
class="td11">ft             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-15-3"  
class="td11">foot           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-15-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-16-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-16-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-16-2"  
class="td11">yd            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-16-3"  
class="td11">yard          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-16-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-17-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-17-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-17-2"  
class="td11">mi            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-17-3"  
class="td11">mile           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-17-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-18-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-18-1"  
class="td11">length      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-18-2"  
class="td11">ly             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-18-3"  
class="td11">lightyear     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-18-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-19-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-19-1"  
class="td11">mass       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-19-2"  
class="td11">g              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-19-3"  
class="td11">gram         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-19-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-20-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-20-1"  
class="td11">mass       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-20-2"  
class="td11">mg           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-20-3"  
class="td11">milligram    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-20-4"  
class="td11">        </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-21-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-21-1"  
class="td11">          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-21-2"  
class="td11">           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-21-3"  
class="td11">            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-21-4"  
class="td11"></td></tr>
<tr  
 style="vertical-align:baseline;" id="TBL-2-22-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-22-1"  
class="td11">mass                  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-22-2"  
class="td11">kg           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-22-3"  
class="td11">kilogram            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-22-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-23-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-23-1"  
class="td11">mass                  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-23-2"  
class="td11">ton          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-23-3"  
class="td11">ton                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-23-4"  
class="td11">imperial/long ton  </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-24-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-24-1"  
class="td11">mass                  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-24-2"  
class="td11">t             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-24-3"  
class="td11">tonne                </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-24-4"  
class="td11">metric/short tonne</td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-25-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-25-1"  
class="td11">mass                  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-25-2"  
class="td11">lb            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-25-3"  
class="td11">pound               </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-25-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-26-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-26-1"  
class="td11">temperature         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-26-2"  
class="td11">Celsius     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-26-3"  
class="td11">degree Celsius     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-26-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-27-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-27-1"  
class="td11">temperature         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-27-2"  
class="td11">Fahrenheit</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-27-3"  
class="td11">degree Fahrenheit</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-27-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-28-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-28-1"  
class="td11">temperature         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-28-2"  
class="td11">K            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-28-3"  
class="td11">kelvin                </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-28-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-29-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-29-1"  
class="td11">force                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-29-2"  
class="td11">N            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-29-3"  
class="td11">newton              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-29-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-30-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-30-1"  
class="td11">energy                </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-30-2"  
class="td11">J             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-30-3"  
class="td11">joule                 </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-30-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-31-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-31-1"  
class="td11">energy                </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-31-2"  
class="td11">kWh        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-31-3"  
class="td11">kilowatt-hour      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-31-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-32-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-32-1"  
class="td11">power                 </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-32-2"  
class="td11">W           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-32-3"  
class="td11">watt                 </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-32-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-33-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-33-1"  
class="td11">electrical current   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-33-2"  
class="td11">A            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-33-3"  
class="td11">ampere              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-33-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-34-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-34-1"  
class="td11">electromotive force</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-34-2"  
class="td11">V            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-34-3"  
class="td11">volt                  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-34-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-35-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-35-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-35-2"  
class="td11">m2          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-35-3"  
class="td11">square meter      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-35-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-36-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-36-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-36-2"  
class="td11">mm2        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-36-3"  
class="td11">square millimeter </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-36-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-37-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-37-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-37-2"  
class="td11">cm2         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-37-3"  
class="td11">square centimeter</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-37-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-38-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-38-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-38-2"  
class="td11">km2        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-38-3"  
class="td11">square kilometer  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-38-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-39-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-39-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-39-2"  
class="td11">in2          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-39-3"  
class="td11">square inch         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-39-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-40-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-40-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-40-2"  
class="td11">ft2          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-40-3"  
class="td11">square foot         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-40-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-41-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-41-1"  
class="td11">area                   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-41-2"  
class="td11">mi2         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-41-3"  
class="td11">square mile        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-41-4"  
class="td11">                </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-42-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-42-1"  
class="td11">                </td></tr>
                                                                          

                                                                          
<tr  
 style="vertical-align:baseline;" id="TBL-2-43-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-43-1"  
class="td11">volume              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-43-2"  
class="td11">m3      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-43-3"  
class="td11">cubic meter                     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-43-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-44-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-44-1"  
class="td11">volume              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-44-2"  
class="td11">cm3     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-44-3"  
class="td11">cubic centimeter               </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-44-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-45-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-45-1"  
class="td11">volume              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-45-2"  
class="td11">L        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-45-3"  
class="td11">liter                               </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-45-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-46-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-46-1"  
class="td11">volume              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-46-2"  
class="td11">gal       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-46-3"  
class="td11">gallon                            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-46-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-47-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-47-1"  
class="td11">pressure            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-47-2"  
class="td11">Pa       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-47-3"  
class="td11">pascal                            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-47-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-48-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-48-1"  
class="td11">pressure            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-48-2"  
class="td11">hPa     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-48-3"  
class="td11">hectopascal                     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-48-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-49-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-49-1"  
class="td11">pressure            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-49-2"  
class="td11">mbar    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-49-3"  
class="td11">millibar                          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-49-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-50-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-50-1"  
class="td11">plane angle        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-50-2"  
class="td11">rad      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-50-3"  
class="td11">radian                            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-50-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-51-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-51-1"  
class="td11">solid angle         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-51-2"  
class="td11">sr        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-51-3"  
class="td11">steradian                        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-51-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-52-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-52-1"  
class="td11">frequency          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-52-2"  
class="td11">Hz       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-52-3"  
class="td11">hertz                              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-52-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-53-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-53-1"  
class="td11">humidity           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-53-2"  
class="td11">g/kg    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-53-3"  
class="td11">absolute humidity             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-53-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-54-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-54-1"  
class="td11">humidity           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-54-2"  
class="td11">RH%   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-54-3"  
class="td11">relative humidity              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-54-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-55-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-55-1"  
class="td11">percentage         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-55-2"  
class="td11">%        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-55-3"  
class="td11">percent                           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-55-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-56-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-56-1"  
class="td11">intensity            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-56-2"  
class="td11">dB       </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-56-3"  
class="td11">decibel                           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-56-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-57-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-57-1"  
class="td11">loudness            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-57-2"  
class="td11">dB SPL</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-57-3"  
class="td11">decibel sound pressure level</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-57-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-58-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-58-1"  
class="td11">loudness            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-58-2"  
class="td11">dBA    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-58-3"  
class="td11">A-weighted decibel           </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-58-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-59-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-59-1"  
class="td11">electric charge    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-59-2"  
class="td11">C        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-59-3"  
class="td11">coulomb                         </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-59-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-60-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-60-1"  
class="td11">electric charge    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-60-2"  
class="td11">Ah      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-60-3"  
class="td11">ampere-hour                    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-60-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-61-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-61-1"  
class="td11">capacitance        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-61-2"  
class="td11">F        </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-61-3"  
class="td11">farad                              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-61-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-62-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-62-1"  
class="td11">electric resistance</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-62-2"  
class="td11">ohm     </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-62-3"  
class="td11">ohm                               </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-62-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-63-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-63-1"  
class="td11">               </td></tr>
<tr  
 style="vertical-align:baseline;" id="TBL-2-64-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-64-1"  
class="td11">electric conductance  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-64-2"  
class="td11">S   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-64-3"  
class="td11">siemens            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-64-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-65-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-65-1"  
class="td11">magnetic flux          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-65-2"  
class="td11">Wb</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-65-3"  
class="td11">weber               </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-65-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-66-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-66-1"  
class="td11">magnetic flux density</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-66-2"  
class="td11">T   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-66-3"  
class="td11">tesla                </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-66-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-67-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-67-1"  
class="td11">inductance              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-67-2"  
class="td11">H   </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-67-3"  
class="td11">henry               </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-67-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-68-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-68-1"  
class="td11">luminous intensity    </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-68-2"  
class="td11">cd  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-68-3"  
class="td11">candela            </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-68-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-69-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-69-1"  
class="td11">luminous flux          </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-69-2"  
class="td11">lm </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-69-3"  
class="td11">lumen              </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-69-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-70-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-70-1"  
class="td11">illuminance             </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-70-2"  
class="td11">lx  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-70-3"  
class="td11">lux                  </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-70-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-71-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-71-1"  
class="td11">catalytic activity      </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-71-2"  
class="td11">kat </td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-71-3"  
class="td11">catalytic activity</td><td  style="white-space:nowrap; text-align:left;" id="TBL-2-71-4"  
class="td11"></td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-72-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-72-1"  
class="td11">                  </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-73-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-73-1"  
class="td11">                  </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-74-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-74-1"  
class="td11">                  </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-75-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-75-1"  
class="td11">                  </td>
</tr><tr  
 style="vertical-align:baseline;" id="TBL-2-76-"><td  style="white-space:nowrap; text-align:left;" id="TBL-2-76-1"  
class="td11">                  </td></tr>
   </table></div>
<p>
   </p> 

   <h3 class="likesectionHead"><a 
 id="x1-15000"></a>Copyright</h3>
<p>Copyright 2013, Sony Computer Science Laboratory Paris. This documentation is
available under the <a 
href="http://www.gnu.org/licenses/fdl.html" >GNU Free Documentation License</a>.
    


    </div> <!-- screen  -->
  </body>
</html>
