var sheet = SpreadsheetApp.openById('YOUR_GOOGLE_SHEET_ID').getActiveSheet();

function doGet(e){
  if(e.parameter.value != 'undefined'){
    var rowData = [];
    rowData[0] = SpreadsheetApp.getActiveSheet().getRange(1, 1).getValue();
    rowData[1] = SpreadsheetApp.getActiveSheet().getRange(1, 2).getValue();
    for(var param in e.parameter){
      var value = stripQuotes(e.parameter[param]);
        switch(param){
          case 's1':
            rowData[0] = value;
            break;
          case 's2':
            rowData[1] = value;
            break;
        }
    }
    
    var newRange = sheet.getRange(1,1,1,rowData.length);
    newRange.setValues([rowData]);
  }
  return ContentService.createTextOutput('ok');
}

function stripQuotes(value){
  return value.replace(/^["']|['"]$/g,"");
}
