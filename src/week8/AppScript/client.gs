var sheet = SpreadsheetApp.openById('YOUR_GOOGLE_SHEET_ID').getActiveSheet();

function doGet(e){
  if(e.parameter.value != 'undefined'){
    var rowData = [];

    for(var param in e.parameter){
      var value = stripQuotes(e.parameter[param]);
      rowData[0] = new Date();
      rowData[1] = Utilities.formatDate(new Date(),"Asia/Bangkok","HH:mm:ss");
        switch(param){
          case 'temp':
            rowData[2] = value;
            break;
          case 'humi':
            rowData[3] = value;
            break;
        }
    }
    var newRow = sheet.getLastRow()+1
    var newRange = sheet.getRange(newRow,1,1,rowData.length);
    newRange.setValues([rowData]);
  }
  return ContentService.createTextOutput('ok');
}

function stripQuotes(value){
  return value.replace(/^["']|['"]$/g,"");
}

