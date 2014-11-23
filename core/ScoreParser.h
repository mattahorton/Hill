#ifndef _SCORE_PARSER_H_
#define _SCORE_PARSER_H_

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class ScoreParser
{

  public:
    ScoreParser ();
    void dumpContents();

  public:
    Document score;

};

#endif
