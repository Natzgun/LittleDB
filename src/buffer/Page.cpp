#include "buffer/Page.h"

#include <iostream>

Page::Page() : capacity(0), namePath(""), pageID(-1) {}

Page::Page(int size, string name) : capacity(size), namePath(name) {}

int Page::getSize() { return capacity; }

void Page::setSize(int size) { capacity = size; }

int Page::getPageId() { return this->pageID; }

void Page::setPageId(int pageId) { this->pageID = pageId; }

string Page::getName() { return namePath; }

void Page::setName(string name) { namePath = name; }
bool Page::addRecordInContent(string &record) {
  if (record.size() > capacity) {
    return false;
  }

  contentRFL += record + "\n";
  capacity -= record.size();

  return true;
}

bool Page::deleteRecordInContent(int index) {
  if (index < 0 || index >= contentRFL.size()) {
    return false;
  }

  contentRFL.erase(index, 1);

  return true;
}

string &Page::getContent() { return contentRFL; }

void Page::setContentRFL(string content) { this->contentRFL = content; }
