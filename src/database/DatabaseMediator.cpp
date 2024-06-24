#include "database/DatabaseMediator.h"
#include "buffer/BufferManager.h"

void mediator::addRecord(string& relation, string record) {
  int pageID;
  cin >> pageID;
  buffManager.addRecordInBuffer(pageID, record);
  
}


