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
function PhotostreamViewer(photos, divname)
{
        var self = this;

        if (photos.length == 0)
                return;

//    alert(photos);

        // Thanks to Scott Andrew LePera
        // (http://www.scottandrew.com/weblog/articles/cbs-events) and
        // Christian Heilmann
        // (http://onlinetools.org/articles/unobtrusivejavascript/chapter4.html)
        // for the setEventHandler() and removeEventHandler() functions.
        this.setEventHandler = function(obj, evType, fn) { 
                if (obj.addEventListener){ 
                        obj.addEventListener(evType, fn, false); 
                        return true; 

                } else if (obj.attachEvent){ 
                        var r = obj.attachEvent("on"+evType, fn); 
                        return r; 

                } else { 
                        return false; 
                } 
        }

        this.insertLink = function(p, anchor, className, handler) {
                var span = document.createElement("SPAN");
                span.className = className;
                p.appendChild(span);        
                var a = document.createElement("A");
                a.setAttribute("href", "javascript:void(0)");
                a.appendChild(document.createTextNode(anchor));
                span.appendChild(a);        
                a.className = className;
                a.onclick = function() { return false; }
                a.onmousedown = function() { return false; }
                this.setEventHandler(a, "click", handler);
                return a;
        }

        this.PhotostreamPhoto = function(viewer, i) {
                var self = this;
                this.viewer = viewer;        
                this.i = i;        
                this.select = function() {
                        self.viewer.selectIcon(self.i);
                }
        }

        this.nextPhoto = function() {
                self.selectPhoto(self.curPhoto + 1);
        }

        this.prevPhoto = function() {
                self.selectPhoto(self.curPhoto - 1);
        }

        this.selectPhoto = function(i) {
                this.curPhoto = i;
                if (this.curPhoto < 1) 
                        this.curPhoto = 0;
                if (this.curPhoto >= this.photos.length) 
                        this.curPhoto = this.photos.length - 1;
                this.doTransition();
        }

        this.selectIcon = function(i) {
                this.curPhoto = this.curOffset + i;
                if (this.curPhoto < 1) 
                        this.curPhoto = this.photos.length - 1;
                if (this.curPhoto >= this.photos.length) 
                        this.curPhoto = 0;
                this.doTransition();
        }

        this.doTransition = function() {
                this.largeImage.src = this.photos[this.curPhoto].small;
                this.largeImage.alt = this.photos[this.curPhoto].datetime;
                this.imageLink.setAttribute("href", this.photos[this.curPhoto].orig);
                this.captionDiv.innerHTML = this.photos[this.curPhoto].datetime;
                var start = this.curPhoto - 2;
                if (start < 0) start = 0;
                this.curOffset = start;
                for (i = 0; i < 5; i++) {
                        if (start + i < this.photos.length) 
                                this.icons[i].src = this.photos[start + i].thumb;
                        if (start + i == this.curPhoto) 
                                this.icons[i].style.border = "solid 1px black";
                        else
                                this.icons[i].style.border = "solid 1px white";
                } 
        }

        this.photos = photos;
        this.curPhoto = 0;
        this.rootDiv = document.getElementById(divname);

        var imageDiv = document.createElement("DIV");
        imageDiv.style.width = 240;
        imageDiv.style.height = 180;
        imageDiv.className = "PhotostreamViewerImageDiv";

        this.imageLink = document.createElement("A");
        this.imageLink.setAttribute("href", "#");
        imageDiv.appendChild(this.imageLink);        

        this.rootDiv.appendChild(imageDiv);        
        this.largeImage = document.createElement("IMG");        
        this.largeImage.src = this.photos[this.curPhoto].small;
        this.largeImage.className = "PhotostreamViewerImage";
        this.imageLink.appendChild(this.largeImage);        

        this.captionDiv = document.createElement("DIV");
        this.captionDiv.style.width = 240;
        this.captionDiv.style.height = 16;
        this.captionDiv.style.textAlign = "center";
        this.captionDiv.className = "PhotostreamViewerCaption";
        this.rootDiv.appendChild(this.captionDiv);        
        this.captionDiv.innerHTML = this.photos[this.curPhoto].datetime;

        var controlDiv = document.createElement("DIV");
        controlDiv.style.width = 240;
        controlDiv.style.height = 32;
        controlDiv.className = "PhotostreamViewerControl";
        this.rootDiv.appendChild(controlDiv);        
        this.insertLink(controlDiv, "<", "PhotostreamViewerArrow", this.prevPhoto);
        this.icons = [];
        this.iconLinks = [];
        for (i = 0; i < 5; i++) {
                if (i < photos.length) {
                        this.iconLinks[i] = document.createElement("A");      
                        this.iconLinks[i].setAttribute("href", "javascript:void(0)");
                        this.iconLinks[i].onclick = function() { return false; }
                        this.iconLinks[i].onmousedown = function() { return false; }
                        controlDiv.appendChild(this.iconLinks[i]);
                        this.setEventHandler(this.iconLinks[i], "click", 
                                        new this.PhotostreamPhoto(this, i).select);

                        this.icons[i] = document.createElement("IMG");        
                        this.icons[i].src = this.photos[i].thumb;
                        this.icons[i].style.width = 32;
                        this.icons[i].style.height = 24;
                        this.icons[i].style.padding = "2px";
                        this.icons[i].style.border = "solid 1px white";
                        this.icons[i].className = "PhotostreamViewerIcon";
                        this.iconLinks[i].appendChild(this.icons[i]);  
                }      
        }
        this.insertLink(controlDiv, ">", "PhotostreamViewerArrow", this.nextPhoto);
        this.doTransition();
}
