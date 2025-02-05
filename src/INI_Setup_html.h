const char INI_Setup_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <head>
  <title>INI WiFiManager</title>
  <meta http-equiv="content-type" content="text/html; charset=UTF-8">
  <link rel="Icon" type="image/ico" href="favicon.ico">
  <style>
  body {
   background-color: #D0D0D4;
   font-family: Arial, Helvetica, sans-serif;   
   text-align: center; 
}
h1 {
   color: navy;
   margin-left: 20px;
   font-size: 150%
}

.button {
   border-top: 1px solid #96d1f8;
   background: #65a9d7;
   background: -webkit-gradient(linear, left top, left bottom, from(#3e779d), to(#65a9d7));
   background: -webkit-linear-gradient(top, #3e779d, #65a9d7);
   background: -moz-linear-gradient(top, #3e779d, #65a9d7);
   background: -ms-linear-gradient(top, #3e779d, #65a9d7);
   background: -o-linear-gradient(top, #3e779d, #65a9d7);
   padding: 5.5px 11px;
   -webkit-border-radius: 8px;
   -moz-border-radius: 8px;
   border-radius: 8px;
   -webkit-box-shadow: rgba(0,0,0,1) 0 1px 0;
   -moz-box-shadow: rgba(0,0,0,1) 0 1px 0;
   box-shadow: rgba(0,0,0,1) 0 1px 0;
   text-shadow: rgba(0,0,0,.4) 0 1px 0;
   color: white;
   font-size: 16px;
   font-family: Georgia, serif;
   text-decoration: none;
   vertical-align: middle;
   }
.button:hover {
   border-top-color: #28597a;
   background: #28597a;
   color: #ffffff;
   }
.button:active {
   border-top-color: #bf157e;
   background: #bf157e;
   }

.buttonr {color: #F59FA0;}
input[type="text"] {
   margin: 0;
   height: 28px;
   background: white;
   font-size: 16px;
   appearance: none;
   box-shadow: none;
   border-radius: 5px;
   -webkit-border-radius: 5px;
   -moz-border-radius: 5px;
   -webkit-appearance: none;
   border: 1px solid black;
   border-radius: 10px;
}

input[type="text"]:focus {
 outline: none;
}

textarea {
  max-width: 99%;
  width: 900px;
  line-height:1.4;
  border: 1px solid black;
  border-radius: 10px;
  // padding: 5px;
  font-family: Courier, "Lucida Console", monospace;
  background-color: white;
  resize: none;
  }
  </style>
 </head>
 
 <body>
   <textarea rows="15" cols="100" id="inifile">INI-Datei wird geladen</textarea> 
   <br><br>
   <button class="button" onclick="fsav()">Speichern</button>   
   &nbsp;&nbsp;
   <button class="button buttonr" onclick="httpGet('reset=0')">Neustart</button>
   <form action="#" onsubmit="return uploadfile(this);" enctype="multipart/form-data" method="post" name="fileinfo">
     <h1>Datei hochladen:</h1>
     <input type="file" name="file" size="50"><br>
     <input class="button" type="submit" value="Senden">
    </form>
<script>
      function httpGet ( theReq )
      {
        var theUrl = "/edit?" + theReq + "&version=" + Math.random() ;
        var xhr = new XMLHttpRequest() ;
        xhr.onreadystatechange = function() {
          if ( xhr.readyState == XMLHttpRequest.DONE )
          {
            //resultstr.value = xhr.responseText ;
          }
        }
        xhr.open ( "GET", theUrl, false ) ;
        xhr.send() ;
      }

      function fsav()
      {
        var theUrl = "/edit" ;
        var xhr = new XMLHttpRequest() ;
        xhr.onreadystatechange = function() {
          if ( xhr.readyState == XMLHttpRequest.DONE )
          {
            //resultstr.value = xhr.responseText ;
          }
        }
        xhr.open ( "POST", theUrl, true ) ;
        xhr.setRequestHeader ( "Content-type", "application/x-www-form-urlencoded" ) ;
        xhr.send ( "content=" + inifile.value ) ;
      }

      function uploadfile ( theForm )
      {
        var oData, oReq ;

        oData = new FormData ( fileinfo ) ;
        oReq = new XMLHttpRequest() ;

        oReq.open ( "POST", "/upload", true ) ;
        oReq.onload = function ( oEvent ) {
          if ( oReq.status == 200 )
          {
            //resultstr.value = oReq.responseText ;
            INIladen ();
          }
          else
          {
            //resultstr.value = "Error " + oReq.statusText ;
          }
        }
        oReq.send ( oData ) ;
        return false ;
      }
      function INIladen ()
      {
       var xhr = new XMLHttpRequest() ;
      theUrl = "/setup.ini" ;
      xhr.onreadystatechange = function() {
        if ( xhr.readyState == XMLHttpRequest.DONE )
        {
          inifile.value = xhr.responseText ;
        }
      }
      xhr.open ( "GET", theUrl, false ) ;
      xhr.send() ;
      }
      INIladen ();
    </script>
    
  </body>
</html>

)=====" ;