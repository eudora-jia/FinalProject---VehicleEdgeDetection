var req;
function reloadGraph() {
   var now = new Date();
url = 'count.txt?' + now.getTime();


   try {

      req = new XMLHttpRequest();

   } catch (e) {

      try {

         req = new ActiveXObject("Msxml2.XMLHTTP");

      } catch (e) {

         try {

            req = new ActiveXObject("Microsoft.XMLHTTP");

         } catch (oc) {

            alert("No AJAX Support");

            return;

         }

      }

   }



   req.onreadystatechange = processReqChange;

   req.open("GET", url, true);

   req.send(null);

   document.images['graph'].src = 'output.png?' + now.getTime();
   document.images['edge'].src = 'edge.png?' + now.getTime();
   // Start new timer (0.5 min)
   timeoutID = setTimeout('reloadGraph()', 500);
}

function processReqChange()

{

   // If req shows "complete"

   if (req.readyState == 4)

   {

      dataDiv = document.getElementById('currentData');



      // If "OK"

      if (req.status == 200)

      {

         // Set current data text

         dataDiv.innerHTML = req.responseText;



         // Start new timer (1 min)

         timeoutID = setTimeout('reloadData()', 60000);

      }

      else

      {

         // Flag error

         dataDiv.innerHTML = '<p>There was a problem retrieving data: ' + req.statusText + '</p>';

      }

   }

}
