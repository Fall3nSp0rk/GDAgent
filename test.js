var datasource, macro, retcode, numberOfLinesInDatasource, aTagWasFound, keyword;
datasource = "S:\\GoldenfrogIPs.csv"
//datasource = "\\\\file1.ad.datafoundry.com\\private$\\michaelw\\My<SP>Documents\\iMacros\Datasources\\GoldenfrogIPs.csv";  
numberOfLinesInDatasource = 128;
aTagWasFound = false;

// loop through all lines in datasource
for (var i = 1; i < numberOfLinesInDatasource+1; i++)
{
 // get the datasource value at this line
   macro = "CODE:";
   macro += "SET !DATASOURCE " + datasource + "\n";
   macro += "SET !DATASOURCE_COLUMNS 1\n";
   macro += "SET !DATASOURCE_LINE " + i + "\n";
   macro += "ADD !EXTRACT {{!COL1}}\n";
   retcode = iimPlay(macro);
   keyword = iimGetLastExtract(0);
//    alert(keyword);                                   // this is coming back blank
   // search for this keyword
   macro = "CODE:";
   macro += "SET !TIMEOUT_PAGE 1\n";                 // added just to speed things up a bit
   macro += "TAG POS=1 TYPE=PRE ATTR=TXT:*" + keyword + "*\n";
   retcode = iimPlay(macro);
   // if retcode is 1 then the tag was found move on to next step
   if (retcode === 1){
       // move on to next step
       aTagWasFound = true;
       break;
   }
   // tag not found try the next value
}
if (!aTagWasFound)
{
   alert("Oh My God Becky! It's NOT a GoldenFrog Ticket!!!");
} else
{
   macro = "CODE:";
   macro += "TAG POS=1 TYPE=A ATTR=TXT:Open\n";
   macro += "TAG POS=1 TYPE=A ATTR=TXT:Take\n";
   macro += "TAG POS=1 TYPE=A ATTR=TXT:Relationships\n";
   macro += "TAG POS=1 TYPE=INPUT:TEXT FORM=ACTION:ModifyLinks.html ATTR=NAME:******-RefersTo CONTENT=c://1814\n";
   macro += "TAG POS=1 TYPE=INPUT:SUBMIT FORM=ACTION:ModifyLinks.html ATTR=*\n";
   macro += "TAG POS=1 TYPE=A ATTR=TXT:Resolve\n";
   macro += "TAG POS=1 TYPE=TEXTAREA FORM=NAME:TicketUpdate ATTR=NAME:UpdateContent CONTENT=IP<SP>is<SP>associated<SP>with<SP>Golden<SP>Frog,<SP>email<SP>has<SP>been<SP>forwarded<SP>to<SP>dmca2010@goldenfrog.com<SP>Resolving\n";
   macro += "TAG POS=1 TYPE=INPUT:TEXT FORM=NAME:TicketUpdate ATTR=NAME:UpdateSubject CONTENT=Golden<SP>Frog<SP>[1814]<SP>DMCA<SP>Notification\n";
   macro += "TAG POS=1 TYPE=SELECT FORM=NAME:TicketUpdate ATTR=NAME:UpdateType CONTENT=%private\n";
   macro += "TAG POS=1 TYPE=INPUT:TEXT FORM=NAME:TicketUpdate ATTR=NAME:UpdateTimeWorked CONTENT=2\n";
   macro += "TAG POS=1 TYPE=INPUT:SUBMIT FORM=NAME:TicketUpdate ATTR=*\n";
   macro += "TAB CLOSE\n";
   retcode = iimPlay(macro);    
} 
