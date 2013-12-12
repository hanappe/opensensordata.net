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

function DatapointConverter(userData, callback)
{
    this.userData = userData;
    this.userCallback = callback;

    this.callback = function (me, data) {
        if (data && data.length) {
            for (i = 0; i < data.length; i++) {
                data[i][0] = new Date(data[i][0]);
            }
        }
        me.userCallback(me.userData, data);        
    }
}

function OpenSensorData(rootURL)
{
    this.root = rootURL;

    this.newXMLHttpRequest = function(userData, callback) {
	var xmlhttp = undefined;

	try {
	    xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
	} catch (e) {
	    try {
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	    } catch (e2) {
		xmlhttp = undefined;
	    }
	}
	if ((xmlhttp == undefined) 
	    && (typeof XMLHttpRequest != 'undefined')) {
	    xmlhttp = new XMLHttpRequest();
	}
        xmlhttp.userData = userData;
        xmlhttp.callback = callback;
        xmlhttp.async = (callback == undefined)? false : true;
	return xmlhttp;
    }

    this.handleResponse = function(xmlhttp) {
        xmlhttp.result = JSON.parse(xmlhttp.response);
        if (xmlhttp.async) 
            xmlhttp.callback(xmlhttp.userData, xmlhttp.result);
    }

    this.handleStateChange = function(xmlhttp) {
	if (xmlhttp.readyState == 4) {
	    if (xmlhttp.status == 200) {
		this.handleResponse(xmlhttp);
	    } else {
		alert("Request for " + xmlhttp._url + " failed: Status " + xmlhttp.status);
	    }
	    delete xmlhttp;
	}
    }
    
    this.get = function(url, userData, callback) {
	var self = this;
	var xmlhttp = this.newXMLHttpRequest(userData, callback);
        xmlhttp._url = this.root + "/" + url;
	xmlhttp.open("GET", this.root + "/" + url, xmlhttp.async);
        //alert(this.root + "/" + url);
        if (xmlhttp.async) {
	    xmlhttp.onreadystatechange = function() {
	        self.handleStateChange(xmlhttp);
	    }
        }
	xmlhttp.send();
        if (!xmlhttp.async) {
            var r = JSON.parse(xmlhttp.response);
            delete xmlhttp;
            return r;
        }
    }

    this.put = function(url, obj, userData, callback) {
	var self = this;
	var xmlhttp = this.newXMLHttpRequest(userData, callback);
	xmlhttp.open("PUT", this.root + "/" + url, xmlhttp.async);
	xmlhttp.onreadystatechange = function() {
	    self.handleStateChange(xmlhttp);
	}
	xmlhttp.send(JSON.stringify(obj));
        if (!xmlhttp.async) {
            var r = xmlhttp.result;
            delete xmlhttp;
            return r;
        }
    }

    this.getdata = function(path, userData, callback) {
        converter = new DatapointConverter(userData, callback);
        this.get(path, converter, converter.callback);
    }
    
    this.getCSV = function(path) {
	var self = this;
	var xmlhttp = this.newXMLHttpRequest(false, false);
	xmlhttp.open("GET", this.root + "/" + path, false);
	xmlhttp.send();
        var r = xmlhttp.responseText;
        delete xmlhttp;
        return r;
    }
}
