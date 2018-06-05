var   SNAPSHOT_FILE = 'https://raw.githubusercontent.com/AACIO/genesis/master/snapshot.csv'
var   snapshot = null
      genesis = null

window.onload = function() { init() }

var init = function() { document.body.id = "loading", snapshot_load() }

var snapshot_load = function() {
  if( check_url(SNAPSHOT_FILE) )
    snapshot_load_csv( SNAPSHOT_FILE, response => {
      if(response)
        SNAPSHOT = snapshot_csv_to_array(response),
        setTimeout( () => document.body.id = "ready", 2500 )
    })
  else
    document.body.id = "error"
}

var snapshot_csv_to_array = function(csv){
  var rows = csv.split(/\r\n|\n/)
  var output = []

  for (var i=0; i<rows.length; i++) {
    var _registrant
        ,registrant = rows[i].replace(/"/g, '').split(',')
        ,[eth, aac, balance] = registrant
    if(eth && aac && balance)
      _registrant = { eth: eth, aac: aac, balance: balance },
      output.push(_registrant)
  }
  return output
}

var snapshot_load_csv = function(filename, callback){
    var xobj = new XMLHttpRequest()
        xobj.overrideMimeType("application/json")
    xobj.open('GET', filename, true); // Replace 'my_data' with the path to your file
    xobj.onreadystatechange = function () {
          if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            callback(xobj.responseText)
          }
    };
    xobj.send(null);
 }

var check_url = function(uri){
  var http
  if (window.XMLHttpRequest)
    http = new XMLHttpRequest()
  else
    http = new ActiveXObject("Microsoft.XMLHTTP")
  http.open('HEAD', uri, false)
  http.send()
  return http.status!=404
}

var bind_snapshot_upload = function() {
  document.forms['snapshot-upload'].elements['snapshot-file'].onchange = function(event) {
    if(!window.FileReader) return // Browser is not compatible
    var reader = new FileReader()
    reader.onload = function(event) {
      if(event.target.readyState != 2) return
      if(event.target.error) {
          alert('Error while reading file')
          return
      }
      genesis = event.target.result
    };
    reader.readAsText(event.target.files[0])
  };
}
