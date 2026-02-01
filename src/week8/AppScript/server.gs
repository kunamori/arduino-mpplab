var sheet = SpreadsheetApp.openById('YOUR_GOOGLE_SHEET_ID').getActiveSheet();
function goGet{
  var data = SpreadsheetApp.getActiveSheet().getRange(1, 1).getValue();
  return ContentService.createTextOutput(data)
}
