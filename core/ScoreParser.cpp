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
  assert(score.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

  assert(score.HasMember("track"));
  assert(score["track"].IsString());
  track = score["track"].GetString();
}
