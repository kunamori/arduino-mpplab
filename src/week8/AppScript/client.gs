var sheet = SpreadsheetApp.openById('YOUR_GOOGLE_SHEET_ID').getActiveSheet();

function doGetData(data){
  if(data.parameter.value != 'undefined'){
    var rowData = [];
    for(var param in data.parameter){
      var value = stripQuotes(data.parameter[param]);
        switch(param){
          case 'data':
            rowData[0] = value;
            break;
        }
    }
    var newRange = sheet.getRange(1,1,1,rowData.length);
    newRange.setValue([rowData]);
  }
  return ContentService.createTextOutput('Ok');
}

function stripQuotes(value){
  return value.replace(/^["']|['"]$/g,"");
}
