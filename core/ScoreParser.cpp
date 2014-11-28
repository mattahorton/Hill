#include "ScoreParser.h"
#include "globals.h"
#include <iostream>
using namespace std;
using namespace rapidjson;

ScoreParser::ScoreParser( )
{
}

void ScoreParser::dumpContents(){

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  score.Accept(writer);

  cout << buffer.GetString() << endl;
}

void ScoreParser::initScore() {
  Value::MemberIterator trackMember = score.FindMember("track");
  track = trackMember->value.GetString();
}
