/// Json-cpp amalgated source (http://jsoncpp.sourceforge.net/).
/// It is intented to be used with #include <json/json.h>

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

/*
The JsonCpp library's source code, including accompanying documentation, 
tests and demonstration applications, are licensed under the following
conditions...

The author (Baptiste Lepilleur) explicitly disclaims copyright in all 
jurisdictions which recognize such a disclaimer. In such jurisdictions, 
this software is released into the Public Domain.

In jurisdictions which do not recognize Public Domain property (e.g. Germany as of
2010), this software is Copyright (c) 2007-2010 by Baptiste Lepilleur, and is
released under the terms of the MIT License (see below).

In jurisdictions which recognize Public Domain property, the user of this 
software may choose to accept it either as 1) Public Domain, 2) under the 
conditions of the MIT License (see below), or 3) under the terms of dual 
Public Domain/MIT License conditions described here, as they choose.

The MIT License is about as close to Public Domain as a license can get, and is
described in clear, concise terms at:

   http://en.wikipedia.org/wiki/MIT_License
   
The full text of the MIT License follows:

========================================================================
Copyright (c) 2007-2010 Baptiste Lepilleur

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the L"Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED L"AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
========================================================================
(END LICENSE TEXT)

The MIT license is compatible with both the GPL and commercial
software, affording one all of the rights of Public Domain with the
minor nuisance of being required to keep the above copyright notice
and license text in the source code. Note also that by accepting the
Public Domain L"license" you can re-license your copy using whatever
license you like.

*/

// //////////////////////////////////////////////////////////////////////
// End of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////






#include "json/json.h"


// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_tool.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef LIB_JSONCPP_JSON_TOOL_H_INCLUDED
# define LIB_JSONCPP_JSON_TOOL_H_INCLUDED

/* This header provides common wstring manipulation support, such as UTF-8,
 * portable conversion from/to string...
 *
 * It is an internal header that must not be exposed.
 */

namespace Json {

/// Converts a unicode code-point to UTF-8.
static inline std::wstring 
codePointToUTF8(unsigned int cp)
{
   std::wstring result;
   
   // based on description from http://en.wikipedia.org/wiki/UTF-8

   if (cp <= 0x7f) 
   {
      result.resize(1);
      result[0] = static_cast<wchar_t>(cp);
   } 
   else if (cp <= 0x7FF) 
   {
      result.resize(2);
      result[1] = static_cast<wchar_t>(0x80 | (0x3f & cp));
      result[0] = static_cast<wchar_t>(0xC0 | (0x1f & (cp >> 6)));
   } 
   else if (cp <= 0xFFFF) 
   {
      result.resize(3);
      result[2] = static_cast<wchar_t>(0x80 | (0x3f & cp));
      result[1] = 0x80 | static_cast<wchar_t>((0x3f & (cp >> 6)));
      result[0] = 0xE0 | static_cast<wchar_t>((0xf & (cp >> 12)));
   }
   else if (cp <= 0x10FFFF) 
   {
      result.resize(4);
      result[3] = static_cast<wchar_t>(0x80 | (0x3f & cp));
      result[2] = static_cast<wchar_t>(0x80 | (0x3f & (cp >> 6)));
      result[1] = static_cast<wchar_t>(0x80 | (0x3f & (cp >> 12)));
      result[0] = static_cast<wchar_t>(0xF0 | (0x7 & (cp >> 18)));
   }

   return result;
}


/// Returns true if ch is a control character (in range [0,32[).
static inline bool 
isControlCharacter(wchar_t ch)
{
   return ch > 0 && ch <= 0x1F;
}


enum { 
   /// Constant that specify the size of the buffer that must be passed to uintToString.
   uintToStringBufferSize = 3*sizeof(LargestUInt)+1 
};

// Defines a wchar_t buffer for use with uintToString().
typedef wchar_t UIntToStringBuffer[uintToStringBufferSize];


/** Converts an unsigned integer to string.
 * @param value Unsigned interger to convert to string
 * @param current Input/Output wstring buffer. 
 *        Must have at least uintToStringBufferSize chars free.
 */
static inline void 
uintToString( LargestUInt value, 
              wchar_t *&current )
{
   *--current = 0;
   do
   {
      *--current = char(value % 10) + L'0';
      value /= 10;
   }
   while ( value != 0 );
}

} // namespace Json {

#endif // LIB_JSONCPP_JSON_TOOL_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_tool.h
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_reader.cpp
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
# include <json/reader.h>
# include <json/value.h>
# include L"json_tool.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <utility>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

#if _MSC_VER >= 1400 // VC++ 8.0
#pragma warning( disable : 4996 )   // disable warning about strdup being deprecated.
#endif

namespace Json {

// Implementation of class Features
// ////////////////////////////////

Features::Features()
   : allowComments_( true )
   , strictRoot_( false )
{
}


Features 
Features::all()
{
   return Features();
}


Features 
Features::strictMode()
{
   Features features;
   features.allowComments_ = false;
   features.strictRoot_ = true;
   return features;
}

// Implementation of class Reader
// ////////////////////////////////


static inline bool 
in( Reader::Char c, Reader::Char c1, Reader::Char c2, Reader::Char c3, Reader::Char c4 )
{
   return c == c1  ||  c == c2  ||  c == c3  ||  c == c4;
}

static inline bool 
in( Reader::Char c, Reader::Char c1, Reader::Char c2, Reader::Char c3, Reader::Char c4, Reader::Char c5 )
{
   return c == c1  ||  c == c2  ||  c == c3  ||  c == c4  ||  c == c5;
}


static bool 
containsNewLine( Reader::Location begin, 
                 Reader::Location end )
{
   for ( ;begin < end; ++begin )
      if ( *begin == L'\n'  ||  *begin == L'\r' )
         return true;
   return false;
}


// Class Reader
// //////////////////////////////////////////////////////////////////

Reader::Reader()
   : features_( Features::all() )
{
}


Reader::Reader( const Features &features )
   : features_( features )
{
}


bool
Reader::parse( const std::wstring &document, 
               Value &root,
               bool collectComments )
{
   document_ = document;
   const wchar_t *begin = document_.c_str();
   const wchar_t *end = begin + document_.length();
   return parse( begin, end, root, collectComments );
}


bool
Reader::parse( std::wistream& sin,
               Value &root,
               bool collectComments )
{
   //std::wistream_iterator<wchar_t> begin(sin);
   //std::wistream_iterator<wchar_t> end;
   // Those would allow streamed input from a file, if parse() were a
   // template function.

   // Since std::wstring is reference-counted, this at least does not
   // create an extra copy.
   std::wstring doc;
   std::getline(sin, doc, (wchar_t)EOF);
   return parse( doc, root, collectComments );
}

bool 
Reader::parse( const wchar_t *beginDoc, const wchar_t *endDoc, 
               Value &root,
               bool collectComments )
{
   if ( !features_.allowComments_ )
   {
      collectComments = false;
   }

   begin_ = beginDoc;
   end_ = endDoc;
   collectComments_ = collectComments;
   current_ = begin_;
   lastValueEnd_ = 0;
   lastValue_ = 0;
   commentsBefore_ = L"";
   errors_.clear();
   while ( !nodes_.empty() )
      nodes_.pop();
   nodes_.push( &root );
   
   bool successful = readValue();
   Token token;
   skipCommentTokens( token );
   if ( collectComments_  &&  !commentsBefore_.empty() )
      root.setComment( commentsBefore_, commentAfter );
   if ( features_.strictRoot_ )
   {
      if ( !root.isArray()  &&  !root.isObject() )
      {
         // Set error location to start of doc, ideally should be first token found in doc
         token.type_ = tokenError;
         token.start_ = beginDoc;
         token.end_ = endDoc;
         addError( L"A valid JSON document must be either an array or an object value.",
                   token );
         return false;
      }
   }
   return successful;
}


bool
Reader::readValue()
{
   Token token;
   skipCommentTokens( token );
   bool successful = true;

   if ( collectComments_  &&  !commentsBefore_.empty() )
   {
      currentValue().setComment( commentsBefore_, commentBefore );
      commentsBefore_ = L"";
   }


   switch ( token.type_ )
   {
   case tokenObjectBegin:
      successful = readObject( token );
      break;
   case tokenArrayBegin:
      successful = readArray( token );
      break;
   case tokenNumber:
      successful = decodeNumber( token );
      break;
   case tokenString:
      successful = decodeString( token );
      break;
   case tokenTrue:
      currentValue() = true;
      break;
   case tokenFalse:
      currentValue() = false;
      break;
   case tokenNull:
      currentValue() = Value();
      break;
   default:
      return addError( L"Syntax error: value, object or array expected.", token );
   }

   if ( collectComments_ )
   {
      lastValueEnd_ = current_;
      lastValue_ = &currentValue();
   }

   return successful;
}


void 
Reader::skipCommentTokens( Token &token )
{
   if ( features_.allowComments_ )
   {
      do
      {
         readToken( token );
      }
      while ( token.type_ == tokenComment );
   }
   else
   {
      readToken( token );
   }
}


bool 
Reader::expectToken( TokenType type, Token &token, const wchar_t *message )
{
   readToken( token );
   if ( token.type_ != type )
      return addError( message, token );
   return true;
}


bool 
Reader::readToken( Token &token )
{
   skipSpaces();
   token.start_ = current_;
   wchar_t c = getNextChar();
   bool ok = true;
   switch ( c )
   {
   case L'{':
      token.type_ = tokenObjectBegin;
      break;
   case L'}':
      token.type_ = tokenObjectEnd;
      break;
   case L'[':
      token.type_ = tokenArrayBegin;
      break;
   case L']':
      token.type_ = tokenArrayEnd;
      break;
   case L'"':
      token.type_ = tokenString;
      ok = readString();
      break;
   case L'/':
      token.type_ = tokenComment;
      ok = readComment();
      break;
   case L'0':
   case L'1':
   case L'2':
   case L'3':
   case L'4':
   case L'5':
   case L'6':
   case L'7':
   case L'8':
   case L'9':
   case L'-':
      token.type_ = tokenNumber;
      readNumber();
      break;
   case L't':
      token.type_ = tokenTrue;
      ok = match( L"rue", 3 );
      break;
   case L'f':
      token.type_ = tokenFalse;
      ok = match( L"alse", 4 );
      break;
   case L'n':
      token.type_ = tokenNull;
      ok = match( L"ull", 3 );
      break;
   case L',':
      token.type_ = tokenArraySeparator;
      break;
   case L':':
      token.type_ = tokenMemberSeparator;
      break;
   case 0:
      token.type_ = tokenEndOfStream;
      break;
   default:
      ok = false;
      break;
   }
   if ( !ok )
      token.type_ = tokenError;
   token.end_ = current_;
   return true;
}


void 
Reader::skipSpaces()
{
   while ( current_ != end_ )
   {
      wchar_t c = *current_;
      if ( c == L' '  ||  c == L'\t'  ||  c == L'\r'  ||  c == L'\n' )
         ++current_;
      else
         break;
   }
}


bool 
Reader::match( Location pattern, 
               int patternLength )
{
   if ( end_ - current_ < patternLength )
      return false;
   int index = patternLength;
   while ( index-- )
      if ( current_[index] != pattern[index] )
         return false;
   current_ += patternLength;
   return true;
}


bool
Reader::readComment()
{
   Location commentBegin = current_ - 1;
   wchar_t c = getNextChar();
   bool successful = false;
   if ( c == L'*' )
      successful = readCStyleComment();
   else if ( c == L'/' )
      successful = readCppStyleComment();
   if ( !successful )
      return false;

   if ( collectComments_ )
   {
      CommentPlacement placement = commentBefore;
      if ( lastValueEnd_  &&  !containsNewLine( lastValueEnd_, commentBegin ) )
      {
         if ( c != L'*'  ||  !containsNewLine( commentBegin, current_ ) )
            placement = commentAfterOnSameLine;
      }

      addComment( commentBegin, current_, placement );
   }
   return true;
}


void 
Reader::addComment( Location begin, 
                    Location end, 
                    CommentPlacement placement )
{
   assert( collectComments_ );
   if ( placement == commentAfterOnSameLine )
   {
      assert( lastValue_ != 0 );
      lastValue_->setComment( std::wstring( begin, end ), placement );
   }
   else
   {
      if ( !commentsBefore_.empty() )
         commentsBefore_ += L"\n";
      commentsBefore_ += std::wstring( begin, end );
   }
}


bool 
Reader::readCStyleComment()
{
   while ( current_ != end_ )
   {
      wchar_t c = getNextChar();
      if ( c == L'*'  &&  *current_ == L'/' )
         break;
   }
   return getNextChar() == L'/';
}


bool 
Reader::readCppStyleComment()
{
   while ( current_ != end_ )
   {
      wchar_t c = getNextChar();
      if (  c == L'\r'  ||  c == L'\n' )
         break;
   }
   return true;
}


void 
Reader::readNumber()
{
   while ( current_ != end_ )
   {
      if ( !(*current_ >= L'0'  &&  *current_ <= L'9')  &&
           !in( *current_, L'.', L'e', L'E', L'+', L'-' ) )
         break;
      ++current_;
   }
}

bool
Reader::readString()
{
   wchar_t c = 0;
   while ( current_ != end_ )
   {
      c = getNextChar();
      if ( c == L'\\' )
         getNextChar();
      else if ( c == L'"' )
         break;
   }
   return c == L'"';
}


bool 
Reader::readObject( Token &/*tokenStart*/ )
{
   Token tokenName;
   std::wstring name;
   currentValue() = Value( objectValue );
   while ( readToken( tokenName ) )
   {
      bool initialTokenOk = true;
      while ( tokenName.type_ == tokenComment  &&  initialTokenOk )
         initialTokenOk = readToken( tokenName );
      if  ( !initialTokenOk )
         break;
      if ( tokenName.type_ == tokenObjectEnd  &&  name.empty() )  // empty object
         return true;
      if ( tokenName.type_ != tokenString )
         break;
      
      name = L"";
      if ( !decodeString( tokenName, name ) )
         return recoverFromError( tokenObjectEnd );

      Token colon;
      if ( !readToken( colon ) ||  colon.type_ != tokenMemberSeparator )
      {
         return addErrorAndRecover( L"Missing L':' after object member name", 
                                    colon, 
                                    tokenObjectEnd );
      }
      Value &value = currentValue()[ name ];
      nodes_.push( &value );
      bool ok = readValue();
      nodes_.pop();
      if ( !ok ) // error already set
         return recoverFromError( tokenObjectEnd );

      Token comma;
      if ( !readToken( comma )
            ||  ( comma.type_ != tokenObjectEnd  &&  
                  comma.type_ != tokenArraySeparator &&
                  comma.type_ != tokenComment ) )
      {
         return addErrorAndRecover( L"Missing L',' or L'}' in object declaration", 
                                    comma, 
                                    tokenObjectEnd );
      }
      bool finalizeTokenOk = true;
      while ( comma.type_ == tokenComment &&
              finalizeTokenOk )
         finalizeTokenOk = readToken( comma );
      if ( comma.type_ == tokenObjectEnd )
         return true;
   }
   return addErrorAndRecover( L"Missing L'}' or object member name", 
                              tokenName, 
                              tokenObjectEnd );
}


bool 
Reader::readArray( Token &/*tokenStart*/ )
{
   currentValue() = Value( arrayValue );
   skipSpaces();
   if ( *current_ == L']' ) // empty array
   {
      Token endArray;
      readToken( endArray );
      return true;
   }
   int index = 0;
   for (;;)
   {
      Value &value = currentValue()[ index++ ];
      nodes_.push( &value );
      bool ok = readValue();
      nodes_.pop();
      if ( !ok ) // error already set
         return recoverFromError( tokenArrayEnd );

      Token token;
      // Accept Comment after last item in the array.
      ok = readToken( token );
      while ( token.type_ == tokenComment  &&  ok )
      {
         ok = readToken( token );
      }
      bool badTokenType = ( token.type_ != tokenArraySeparator  &&
                            token.type_ != tokenArrayEnd );
      if ( !ok  ||  badTokenType )
      {
         return addErrorAndRecover( L"Missing L',' or L']' in array declaration", 
                                    token, 
                                    tokenArrayEnd );
      }
      if ( token.type_ == tokenArrayEnd )
         break;
   }
   return true;
}


bool 
Reader::decodeNumber( Token &token )
{
   bool isDouble = false;
   for ( Location inspect = token.start_; inspect != token.end_; ++inspect )
   {
      isDouble = isDouble  
                 ||  in( *inspect, L'.', L'e', L'E', L'+' )  
                 ||  ( *inspect == L'-'  &&  inspect != token.start_ );
   }
   if ( isDouble )
      return decodeDouble( token );
   // Attempts to parse the number as an integer. If the number is
   // larger than the maximum supported value of an integer then
   // we decode the number as a double.
   Location current = token.start_;
   bool isNegative = *current == L'-';
   if ( isNegative )
      ++current;
   Value::LargestUInt maxIntegerValue = isNegative ? Value::LargestUInt(-Value::minLargestInt) 
                                                   : Value::maxLargestUInt;
   Value::LargestUInt threshold = maxIntegerValue / 10;
   Value::UInt lastDigitThreshold = Value::UInt( maxIntegerValue % 10 );
   assert( lastDigitThreshold >=0  &&  lastDigitThreshold <= 9 );
   Value::LargestUInt value = 0;
   while ( current < token.end_ )
   {
      wchar_t c = *current++;
      if ( c < L'0'  ||  c > L'9' )
         return addError( L"'" + std::wstring( token.start_, token.end_ ) + L"' is not a number.", token );
      Value::UInt digit(c - L'0');
      if ( value >= threshold )
      {
         // If the current digit is not the last one, or if it is
         // greater than the last digit of the maximum integer value,
         // the parse the number as a double.
         if ( current != token.end_  ||  digit > lastDigitThreshold )
         {
            return decodeDouble( token );
         }
      }
      value = value * 10 + digit;
   }
   if ( isNegative )
      currentValue() = -Value::LargestInt( value );
   else if ( value <= Value::LargestUInt(Value::maxInt) )
      currentValue() = Value::LargestInt( value );
   else
      currentValue() = value;
   return true;
}


bool 
Reader::decodeDouble( Token &token )
{
   double value = 0;
   const int bufferSize = 32;
   int count;
   int length = int(token.end_ - token.start_);
   if ( length <= bufferSize )
   {
      wchar_t buffer[bufferSize+1];
      memcpy( buffer, token.start_, length );
      buffer[length] = 0;
      count = swscanf_s( buffer, L"%lf", &value );
   }
   else
   {
      std::wstring buffer( token.start_, token.end_ );
      count = swscanf_s( buffer.c_str(), L"%lf", &value );
   }

   if ( count != 1 )
      return addError( L"'" + std::wstring( token.start_, token.end_ ) + L"' is not a number.", token );
   currentValue() = value;
   return true;
}


bool 
Reader::decodeString( Token &token )
{
   std::wstring decoded;
   if ( !decodeString( token, decoded ) )
      return false;
   currentValue() = decoded;
   return true;
}


bool 
Reader::decodeString( Token &token, std::wstring &decoded )
{
   decoded.reserve( token.end_ - token.start_ - 2 );
   Location current = token.start_ + 1; // skip L'"'
   Location end = token.end_ - 1;      // do not include L'"'
   while ( current != end )
   {
      wchar_t c = *current++;
      if ( c == L'"' )
         break;
      else if ( c == L'\\' )
      {
         if ( current == end )
            return addError( L"Empty escape sequence in string", token, current );
         wchar_t escape = *current++;
         switch ( escape )
         {
         case L'"': decoded += L'"'; break;
         case L'/': decoded += L'/'; break;
         case L'\\': decoded += L'\\'; break;
         case L'b': decoded += L'\b'; break;
         case L'f': decoded += L'\f'; break;
         case L'n': decoded += L'\n'; break;
         case L'r': decoded += L'\r'; break;
         case L't': decoded += L'\t'; break;
         case L'u':
            {
               unsigned int unicode;
               if ( !decodeUnicodeCodePoint( token, current, end, unicode ) )
                  return false;
               decoded += codePointToUTF8(unicode);
            }
            break;
         default:
            return addError( L"Bad escape sequence in string", token, current );
         }
      }
      else
      {
         decoded += c;
      }
   }
   return true;
}

bool
Reader::decodeUnicodeCodePoint( Token &token, 
                                     Location &current, 
                                     Location end, 
                                     unsigned int &unicode )
{

   if ( !decodeUnicodeEscapeSequence( token, current, end, unicode ) )
      return false;
   if (unicode >= 0xD800 && unicode <= 0xDBFF)
   {
      // surrogate pairs
      if (end - current < 6)
         return addError( L"additional six characters expected to parse unicode surrogate pair.", token, current );
      unsigned int surrogatePair;
      if (*(current++) == L'\\' && *(current++)== L'u')
      {
         if (decodeUnicodeEscapeSequence( token, current, end, surrogatePair ))
         {
            unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
         } 
         else
            return false;
      } 
      else
         return addError( L"expecting another \\u token to begin the second half of a unicode surrogate pair", token, current );
   }
   return true;
}

bool 
Reader::decodeUnicodeEscapeSequence( Token &token, 
                                     Location &current, 
                                     Location end, 
                                     unsigned int &unicode )
{
   if ( end - current < 4 )
      return addError( L"Bad unicode escape sequence in string: four digits expected.", token, current );
   unicode = 0;
   for ( int index =0; index < 4; ++index )
   {
      wchar_t c = *current++;
      unicode *= 16;
      if ( c >= L'0'  &&  c <= L'9' )
         unicode += c - L'0';
      else if ( c >= L'a'  &&  c <= L'f' )
         unicode += c - L'a' + 10;
      else if ( c >= L'A'  &&  c <= L'F' )
         unicode += c - L'A' + 10;
      else
         return addError( L"Bad unicode escape sequence in string: hexadecimal digit expected.", token, current );
   }
   return true;
}


bool 
Reader::addError( const std::wstring &message, 
                  Token &token,
                  Location extra )
{
   ErrorInfo info;
   info.token_ = token;
   info.message_ = message;
   info.extra_ = extra;
   errors_.push_back( info );
   return false;
}


bool 
Reader::recoverFromError( TokenType skipUntilToken )
{
   int errorCount = int(errors_.size());
   Token skip;
   for (;;)
   {
      if ( !readToken(skip) )
         errors_.resize( errorCount ); // discard errors caused by recovery
      if ( skip.type_ == skipUntilToken  ||  skip.type_ == tokenEndOfStream )
         break;
   }
   errors_.resize( errorCount );
   return false;
}


bool 
Reader::addErrorAndRecover( const std::wstring &message, 
                            Token &token,
                            TokenType skipUntilToken )
{
   addError( message, token );
   return recoverFromError( skipUntilToken );
}


Value &
Reader::currentValue()
{
   return *(nodes_.top());
}


Reader::Char 
Reader::getNextChar()
{
   if ( current_ == end_ )
      return 0;
   return *current_++;
}


void 
Reader::getLocationLineAndColumn( Location location,
                                  int &line,
                                  int &column ) const
{
   Location current = begin_;
   Location lastLineStart = current;
   line = 0;
   while ( current < location  &&  current != end_ )
   {
      wchar_t c = *current++;
      if ( c == L'\r' )
      {
         if ( *current == L'\n' )
            ++current;
         lastLineStart = current;
         ++line;
      }
      else if ( c == L'\n' )
      {
         lastLineStart = current;
         ++line;
      }
   }
   // column & line start at 1
   column = int(location - lastLineStart) + 1;
   ++line;
}


std::wstring
Reader::getLocationLineAndColumn( Location location ) const
{
   int line, column;
   getLocationLineAndColumn( location, line, column );
   wchar_t buffer[18+16+16+1];
   swprintf_s( buffer, L"Line %d, Column %d", line, column );
   return buffer;
}


// Deprecated. Preserved for backward compatibility
std::string 
Reader::getFormatedErrorMessages() const
{
    return getFormattedErrorMessages();
}


std::string 
Reader::getFormattedErrorMessages() const
{
   std::wstring formattedMessage;
   for ( Errors::const_iterator itError = errors_.begin();
         itError != errors_.end();
         ++itError )
   {
      const ErrorInfo &error = *itError;
      formattedMessage += L"* L" + getLocationLineAndColumn( error.token_.start_ ) + L"\n";
      formattedMessage += L"  L" + error.message_ + L"\n";
      if ( error.extra_ )
         formattedMessage += L"See L" + getLocationLineAndColumn( error.extra_ ) + L" for detail.\n";
   }
   
   std::string formattedMessage_out;
   formattedMessage_out.resize(formattedMessage.size());
   std::copy(formattedMessage.begin(), formattedMessage.end(), formattedMessage_out.begin());

   return formattedMessage_out;
}


std::wistream& operator>>( std::wistream &sin, Value &root )
{
    Json::Reader reader;
    bool ok = reader.parse(sin, root, true);
    //JSON_ASSERT( ok );
    if (!ok) throw std::runtime_error(reader.getFormattedErrorMessages());
    return sin;
}


} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_reader.cpp
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_batchallocator.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSONCPP_BATCHALLOCATOR_H_INCLUDED
# define JSONCPP_BATCHALLOCATOR_H_INCLUDED

# include <stdlib.h>
# include <assert.h>

# ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

namespace Json {

/* Fast memory allocator.
 *
 * This memory allocator allocates memory for a batch of object (specified by
 * the page size, the number of object in each page).
 *
 * It does not allow the destruction of a single object. All the allocated objects
 * can be destroyed at once. The memory can be either released or reused for future
 * allocation.
 * 
 * The in-place new operator must be used to construct the object using the pointer
 * returned by allocate.
 */
template<typename AllocatedType
        ,const unsigned int objectPerAllocation>
class BatchAllocator
{
public:
   typedef AllocatedType Type;

   BatchAllocator( unsigned int objectsPerPage = 255 )
      : freeHead_( 0 )
      , objectsPerPage_( objectsPerPage )
   {
//      printf( L"Size: %d => %s\n", sizeof(AllocatedType), typeid(AllocatedType).name() );
      assert( sizeof(AllocatedType) * objectPerAllocation >= sizeof(AllocatedType *) ); // We must be able to store a slist in the object free space.
      assert( objectsPerPage >= 16 );
      batches_ = allocateBatch( 0 );   // allocated a dummy page
      currentBatch_ = batches_;
   }

   ~BatchAllocator()
   {
      for ( BatchInfo *batch = batches_; batch;  )
      {
         BatchInfo *nextBatch = batch->next_;
         free( batch );
         batch = nextBatch;
      }
   }

   /// allocate space for an array of objectPerAllocation object.
   /// @warning it is the responsability of the caller to call objects constructors.
   AllocatedType *allocate()
   {
      if ( freeHead_ ) // returns node from free list.
      {
         AllocatedType *object = freeHead_;
         freeHead_ = *(AllocatedType **)object;
         return object;
      }
      if ( currentBatch_->used_ == currentBatch_->end_ )
      {
         currentBatch_ = currentBatch_->next_;
         while ( currentBatch_  &&  currentBatch_->used_ == currentBatch_->end_ )
            currentBatch_ = currentBatch_->next_;

         if ( !currentBatch_  ) // no free batch found, allocate a new one
         { 
            currentBatch_ = allocateBatch( objectsPerPage_ );
            currentBatch_->next_ = batches_; // insert at the head of the list
            batches_ = currentBatch_;
         }
      }
      AllocatedType *allocated = currentBatch_->used_;
      currentBatch_->used_ += objectPerAllocation;
      return allocated;
   }

   /// Release the object.
   /// @warning it is the responsability of the caller to actually destruct the object.
   void release( AllocatedType *object )
   {
      assert( object != 0 );
      *(AllocatedType **)object = freeHead_;
      freeHead_ = object;
   }

private:
   struct BatchInfo
   {
      BatchInfo *next_;
      AllocatedType *used_;
      AllocatedType *end_;
      AllocatedType buffer_[objectPerAllocation];
   };

   // disabled copy constructor and assignement operator.
   BatchAllocator( const BatchAllocator & );
   void operator =( const BatchAllocator &);

   static BatchInfo *allocateBatch( unsigned int objectsPerPage )
   {
      const unsigned int mallocSize = sizeof(BatchInfo) - sizeof(AllocatedType)* objectPerAllocation
                                + sizeof(AllocatedType) * objectPerAllocation * objectsPerPage;
      BatchInfo *batch = static_cast<BatchInfo*>( malloc( mallocSize ) );
      batch->next_ = 0;
      batch->used_ = batch->buffer_;
      batch->end_ = batch->buffer_ + objectsPerPage;
      return batch;
   }

   BatchInfo *batches_;
   BatchInfo *currentBatch_;
   /// Head of a single linked list within the allocated space of freeed object
   AllocatedType *freeHead_;
   unsigned int objectsPerPage_;
};


} // namespace Json

# endif // ifndef JSONCPP_DOC_INCLUDE_IMPLEMENTATION

#endif // JSONCPP_BATCHALLOCATOR_H_INCLUDED


// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_batchallocator.h
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_valueiterator.inl
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

// included by json_value.cpp

namespace Json {

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueIteratorBase
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueIteratorBase::ValueIteratorBase()
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   : current_()
   , isNull_( true )
{
}
#else
   : isArray_( true )
   , isNull_( true )
{
   iterator_.array_ = ValueInternalArray::IteratorState();
}
#endif


#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueIteratorBase::ValueIteratorBase( const Value::ObjectValues::iterator &current )
   : current_( current )
   , isNull_( false )
{
}
#else
ValueIteratorBase::ValueIteratorBase( const ValueInternalArray::IteratorState &state )
   : isArray_( true )
{
   iterator_.array_ = state;
}


ValueIteratorBase::ValueIteratorBase( const ValueInternalMap::IteratorState &state )
   : isArray_( false )
{
   iterator_.map_ = state;
}
#endif

Value &
ValueIteratorBase::deref() const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   return current_->second;
#else
   if ( isArray_ )
      return ValueInternalArray::dereference( iterator_.array_ );
   return ValueInternalMap::value( iterator_.map_ );
#endif
}


void 
ValueIteratorBase::increment()
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   ++current_;
#else
   if ( isArray_ )
      ValueInternalArray::increment( iterator_.array_ );
   ValueInternalMap::increment( iterator_.map_ );
#endif
}


void 
ValueIteratorBase::decrement()
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   --current_;
#else
   if ( isArray_ )
      ValueInternalArray::decrement( iterator_.array_ );
   ValueInternalMap::decrement( iterator_.map_ );
#endif
}


ValueIteratorBase::difference_type 
ValueIteratorBase::computeDistance( const SelfType &other ) const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
# ifdef JSON_USE_CPPTL_SMALLMAP
   return current_ - other.current_;
# else
   // Iterator for null value are initialized using the default
   // constructor, which initialize current_ to the default
   // std::map::iterator. As begin() and end() are two instance 
   // of the default std::map::iterator, they can not be compared.
   // To allow this, we handle this comparison specifically.
   if ( isNull_  &&  other.isNull_ )
   {
      return 0;
   }


   // Usage of std::distance is not portable (does not compile with Sun Studio 12 RogueWave STL,
   // which is the one used by default).
   // Using a portable hand-made version for non random iterator instead:
   //   return difference_type( std::distance( current_, other.current_ ) );
   difference_type myDistance = 0;
   for ( Value::ObjectValues::iterator it = current_; it != other.current_; ++it )
   {
      ++myDistance;
   }
   return myDistance;
# endif
#else
   if ( isArray_ )
      return ValueInternalArray::distance( iterator_.array_, other.iterator_.array_ );
   return ValueInternalMap::distance( iterator_.map_, other.iterator_.map_ );
#endif
}


bool 
ValueIteratorBase::isEqual( const SelfType &other ) const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   if ( isNull_ )
   {
      return other.isNull_;
   }
   return current_ == other.current_;
#else
   if ( isArray_ )
      return ValueInternalArray::equals( iterator_.array_, other.iterator_.array_ );
   return ValueInternalMap::equals( iterator_.map_, other.iterator_.map_ );
#endif
}


void 
ValueIteratorBase::copy( const SelfType &other )
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   current_ = other.current_;
#else
   if ( isArray_ )
      iterator_.array_ = other.iterator_.array_;
   iterator_.map_ = other.iterator_.map_;
#endif
}


Value 
ValueIteratorBase::key() const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   const Value::CZString czstring = (*current_).first;
   if ( czstring.c_str() )
   {
      if ( czstring.isStaticString() )
         return Value( StaticString( czstring.c_str() ) );
      return Value( czstring.c_str() );
   }
   return Value( czstring.index() );
#else
   if ( isArray_ )
      return Value( ValueInternalArray::indexOf( iterator_.array_ ) );
   bool isStatic;
   const wchar_t *memberName = ValueInternalMap::key( iterator_.map_, isStatic );
   if ( isStatic )
      return Value( StaticString( memberName ) );
   return Value( memberName );
#endif
}


UInt 
ValueIteratorBase::index() const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   const Value::CZString czstring = (*current_).first;
   if ( !czstring.c_str() )
      return czstring.index();
   return Value::UInt( -1 );
#else
   if ( isArray_ )
      return Value::UInt( ValueInternalArray::indexOf( iterator_.array_ ) );
   return Value::UInt( -1 );
#endif
}


const wchar_t *
ValueIteratorBase::memberName() const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   const wchar_t *name = (*current_).first.c_str();
   return name ? name : L"";
#else
   if ( !isArray_ )
      return ValueInternalMap::key( iterator_.map_ );
   return L"";
#endif
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueConstIterator
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueConstIterator::ValueConstIterator()
{
}


#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueConstIterator::ValueConstIterator( const Value::ObjectValues::iterator &current )
   : ValueIteratorBase( current )
{
}
#else
ValueConstIterator::ValueConstIterator( const ValueInternalArray::IteratorState &state )
   : ValueIteratorBase( state )
{
}

ValueConstIterator::ValueConstIterator( const ValueInternalMap::IteratorState &state )
   : ValueIteratorBase( state )
{
}
#endif

ValueConstIterator &
ValueConstIterator::operator =( const ValueIteratorBase &other )
{
   copy( other );
   return *this;
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueIterator
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueIterator::ValueIterator()
{
}


#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueIterator::ValueIterator( const Value::ObjectValues::iterator &current )
   : ValueIteratorBase( current )
{
}
#else
ValueIterator::ValueIterator( const ValueInternalArray::IteratorState &state )
   : ValueIteratorBase( state )
{
}

ValueIterator::ValueIterator( const ValueInternalMap::IteratorState &state )
   : ValueIteratorBase( state )
{
}
#endif

ValueIterator::ValueIterator( const ValueConstIterator &other )
   : ValueIteratorBase( other )
{
}

ValueIterator::ValueIterator( const ValueIterator &other )
   : ValueIteratorBase( other )
{
}

ValueIterator &
ValueIterator::operator =( const SelfType &other )
{
   copy( other );
   return *this;
}

} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_valueiterator.inl
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_value.cpp
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
# include <json/value.h>
# include <json/writer.h>
# ifndef JSON_USE_SIMPLE_INTERNAL_ALLOCATOR
#  include L"json_batchallocator.h"
# endif // #ifndef JSON_USE_SIMPLE_INTERNAL_ALLOCATOR
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <iostream>
#include <utility>
#include <stdexcept>
#include <cstring>
#include <cassert>
#ifdef JSON_USE_CPPTL
# include <cpptl/conststring.h>
#endif
#include <cstddef>    // size_t

#define JSON_ASSERT_UNREACHABLE assert( false )
#define JSON_ASSERT( condition ) assert( condition );  // @todo <= change this into an exception throw
#define JSON_FAIL_MESSAGE( message ) throw std::runtime_error( message );
#define JSON_ASSERT_MESSAGE( condition, message ) if (!( condition )) JSON_FAIL_MESSAGE( message )

namespace Json {

const Value Value::null;
const Int Value::minInt = Int( ~(UInt(-1)/2) );
const Int Value::maxInt = Int( UInt(-1)/2 );
const UInt Value::maxUInt = UInt(-1);
const Int64 Value::minInt64 = Int64( ~(UInt64(-1)/2) );
const Int64 Value::maxInt64 = Int64( UInt64(-1)/2 );
const UInt64 Value::maxUInt64 = UInt64(-1);
const LargestInt Value::minLargestInt = LargestInt( ~(LargestUInt(-1)/2) );
const LargestInt Value::maxLargestInt = LargestInt( LargestUInt(-1)/2 );
const LargestUInt Value::maxLargestUInt = LargestUInt(-1);


/// Unknown size marker
static const unsigned int unknown = (unsigned)-1;


/** Duplicates the specified wstring value.
 * @param value Pointer to the wstring to duplicate. Must be zero-terminated if
 *              length is L"unknown".
 * @param length Length of the value. if equals to unknown, then it will be
 *               computed using wcslen(value).
 * @return Pointer on the duplicate instance of string.
 */
static inline wchar_t *
duplicateStringValue( const wchar_t *value, 
                      unsigned int length = unknown )
{
   if ( length == unknown )
      length = (unsigned int)wcslen(value);
   wchar_t *newString = static_cast<wchar_t *>( malloc( length * sizeof(wchar_t) + 1 ) );
   JSON_ASSERT_MESSAGE( newString != 0, "Failed to allocate wstring value buffer" );
   memcpy( newString, value, length );
   newString[length] = 0;
   return newString;
}


/** Free the wstring duplicated by duplicateStringValue().
 */
static inline void 
releaseStringValue( wchar_t *value )
{
   if ( value )
      free( value );
}

} // namespace Json


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// ValueInternals...
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
#if !defined(JSON_IS_AMALGAMATION)
# ifdef JSON_VALUE_USE_INTERNAL_MAP
#  include L"json_internalarray.inl"
#  include L"json_internalmap.inl"
# endif // JSON_VALUE_USE_INTERNAL_MAP

# include L"json_valueiterator.inl"
#endif // if !defined(JSON_IS_AMALGAMATION)

namespace Json {

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::CommentInfo
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////


Value::CommentInfo::CommentInfo()
   : comment_( 0 )
{
}

Value::CommentInfo::~CommentInfo()
{
   if ( comment_ )
      releaseStringValue( comment_ );
}


void 
Value::CommentInfo::setComment( const wchar_t *text )
{
   if ( comment_ )
      releaseStringValue( comment_ );
   JSON_ASSERT( text != 0 );
   JSON_ASSERT_MESSAGE( text[0]=='\0' || text[0]=='/', "Comments must start with /");
   // It seems that /**/ style comments are acceptable as well.
   comment_ = duplicateStringValue( text );
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::CZString
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
# ifndef JSON_VALUE_USE_INTERNAL_MAP

// Notes: index_ indicates if the wstring was allocated when
// a wstring is stored.

Value::CZString::CZString( ArrayIndex index )
   : cstr_( 0 )
   , index_( index )
{
}

Value::CZString::CZString( const wchar_t *cstr, DuplicationPolicy allocate )
   : cstr_( allocate == duplicate ? duplicateStringValue(cstr) 
                                  : cstr )
   , index_( allocate )
{
}

Value::CZString::CZString( const CZString &other )
: cstr_( other.index_ != noDuplication &&  other.cstr_ != 0
                ?  duplicateStringValue( other.cstr_ )
                : other.cstr_ )
   , index_( other.cstr_ ? (other.index_ == noDuplication ? noDuplication : duplicate)
                         : other.index_ )
{
}

Value::CZString::~CZString()
{
   if ( cstr_  &&  index_ == duplicate )
      releaseStringValue( const_cast<wchar_t *>( cstr_ ) );
}

void 
Value::CZString::swap( CZString &other )
{
   std::swap( cstr_, other.cstr_ );
   std::swap( index_, other.index_ );
}

Value::CZString &
Value::CZString::operator =( const CZString &other )
{
   CZString temp( other );
   swap( temp );
   return *this;
}

bool 
Value::CZString::operator<( const CZString &other ) const 
{
   if ( cstr_ )
      return wcscmp( cstr_, other.cstr_ ) < 0;
   return index_ < other.index_;
}

bool 
Value::CZString::operator==( const CZString &other ) const 
{
   if ( cstr_ )
      return wcscmp( cstr_, other.cstr_ ) == 0;
   return index_ == other.index_;
}


ArrayIndex 
Value::CZString::index() const
{
   return index_;
}


const wchar_t *
Value::CZString::c_str() const
{
   return cstr_;
}

bool 
Value::CZString::isStaticString() const
{
   return index_ == noDuplication;
}

#endif // ifndef JSON_VALUE_USE_INTERNAL_MAP


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::Value
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

/*! \internal Default constructor initialization must be equivalent to:
 * memset( this, 0, sizeof(Value) )
 * This optimization is used in ValueInternalMap fast allocator.
 */
Value::Value( ValueType type )
   : type_( type )
   , allocated_( 0 )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   switch ( type )
   {
   case nullValue:
      break;
   case intValue:
   case uintValue:
      value_.int_ = 0;
      break;
   case realValue:
      value_.real_ = 0.0;
      break;
   case stringValue:
      value_.string_ = 0;
      break;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      value_.map_ = new ObjectValues();
      break;
#else
   case arrayValue:
      value_.array_ = arrayAllocator()->newArray();
      break;
   case objectValue:
      value_.map_ = mapAllocator()->newMap();
      break;
#endif
   case booleanValue:
      value_.bool_ = false;
      break;
   default:
      JSON_ASSERT_UNREACHABLE;
   }
}


#if defined(JSON_HAS_INT64)
Value::Value( UInt value )
   : type_( uintValue )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.uint_ = value;
}

Value::Value( Int value )
   : type_( intValue )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.int_ = value;
}

#endif // if defined(JSON_HAS_INT64)


Value::Value( Int64 value )
   : type_( intValue )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.int_ = value;
}


Value::Value( UInt64 value )
   : type_( uintValue )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.uint_ = value;
}

Value::Value( double value )
   : type_( realValue )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.real_ = value;
}

Value::Value( const wchar_t *value )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = duplicateStringValue( value );
}


Value::Value( const wchar_t *beginValue, 
              const wchar_t *endValue )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = duplicateStringValue( beginValue, 
                                          (unsigned int)(endValue - beginValue) );
}


Value::Value( const std::wstring &value )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = duplicateStringValue( value.c_str(), 
                                          (unsigned int)value.length() );

}

Value::Value( const StaticString &value )
   : type_( stringValue )
   , allocated_( false )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = const_cast<wchar_t *>( value.c_str() );
}


# ifdef JSON_USE_CPPTL
Value::Value( const CppTL::Constwstring &value )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = duplicateStringValue( value, value.length() );
}
# endif

Value::Value( bool value )
   : type_( booleanValue )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.bool_ = value;
}


Value::Value( const Value &other )
   : type_( other.type_ )
   , comments_( 0 )
# ifdef JSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   switch ( type_ )
   {
   case nullValue:
   case intValue:
   case uintValue:
   case realValue:
   case booleanValue:
      value_ = other.value_;
      break;
   case stringValue:
      if ( other.value_.string_ )
      {
         value_.string_ = duplicateStringValue( other.value_.string_ );
         allocated_ = true;
      }
      else
         value_.string_ = 0;
      break;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      value_.map_ = new ObjectValues( *other.value_.map_ );
      break;
#else
   case arrayValue:
      value_.array_ = arrayAllocator()->newArrayCopy( *other.value_.array_ );
      break;
   case objectValue:
      value_.map_ = mapAllocator()->newMapCopy( *other.value_.map_ );
      break;
#endif
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   if ( other.comments_ )
   {
      comments_ = new CommentInfo[numberOfCommentPlacement];
      for ( int comment =0; comment < numberOfCommentPlacement; ++comment )
      {
         const CommentInfo &otherComment = other.comments_[comment];
         if ( otherComment.comment_ )
            comments_[comment].setComment( otherComment.comment_ );
      }
   }
}


Value::~Value()
{
   switch ( type_ )
   {
   case nullValue:
   case intValue:
   case uintValue:
   case realValue:
   case booleanValue:
      break;
   case stringValue:
      if ( allocated_ )
         releaseStringValue( value_.string_ );
      break;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      delete value_.map_;
      break;
#else
   case arrayValue:
      arrayAllocator()->destructArray( value_.array_ );
      break;
   case objectValue:
      mapAllocator()->destructMap( value_.map_ );
      break;
#endif
   default:
      JSON_ASSERT_UNREACHABLE;
   }

   if ( comments_ )
      delete[] comments_;
}

Value &
Value::operator=( const Value &other )
{
   Value temp( other );
   swap( temp );
   return *this;
}

void 
Value::swap( Value &other )
{
   ValueType temp = type_;
   type_ = other.type_;
   other.type_ = temp;
   std::swap( value_, other.value_ );
   int temp2 = allocated_;
   allocated_ = other.allocated_;
   other.allocated_ = temp2;
}

ValueType 
Value::type() const
{
   return type_;
}


int 
Value::compare( const Value &other ) const
{
   if ( *this < other )
      return -1;
   if ( *this > other )
      return 1;
   return 0;
}


bool 
Value::operator <( const Value &other ) const
{
   int typeDelta = type_ - other.type_;
   if ( typeDelta )
      return typeDelta < 0 ? true : false;
   switch ( type_ )
   {
   case nullValue:
      return false;
   case intValue:
      return value_.int_ < other.value_.int_;
   case uintValue:
      return value_.uint_ < other.value_.uint_;
   case realValue:
      return value_.real_ < other.value_.real_;
   case booleanValue:
      return value_.bool_ < other.value_.bool_;
   case stringValue:
      return ( value_.string_ == 0  &&  other.value_.string_ )
             || ( other.value_.string_  
                  &&  value_.string_  
                  && wcscmp( value_.string_, other.value_.string_ ) < 0 );
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      {
         int delta = int( value_.map_->size() - other.value_.map_->size() );
         if ( delta )
            return delta < 0;
         return (*value_.map_) < (*other.value_.map_);
      }
#else
   case arrayValue:
      return value_.array_->compare( *(other.value_.array_) ) < 0;
   case objectValue:
      return value_.map_->compare( *(other.value_.map_) ) < 0;
#endif
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return false;  // unreachable
}

bool 
Value::operator <=( const Value &other ) const
{
   return !(other < *this);
}

bool 
Value::operator >=( const Value &other ) const
{
   return !(*this < other);
}

bool 
Value::operator >( const Value &other ) const
{
   return other < *this;
}

bool 
Value::operator ==( const Value &other ) const
{
   //if ( type_ != other.type_ )
   // GCC 2.95.3 says:
   // attempt to take address of bit-field structure member `Json::Value::type_'
   // Beats me, but a temp solves the problem.
   int temp = other.type_;
   if ( type_ != temp )
      return false;
   switch ( type_ )
   {
   case nullValue:
      return true;
   case intValue:
      return value_.int_ == other.value_.int_;
   case uintValue:
      return value_.uint_ == other.value_.uint_;
   case realValue:
      return value_.real_ == other.value_.real_;
   case booleanValue:
      return value_.bool_ == other.value_.bool_;
   case stringValue:
      return ( value_.string_ == other.value_.string_ )
             || ( other.value_.string_  
                  &&  value_.string_  
                  && wcscmp( value_.string_, other.value_.string_ ) == 0 );
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      return value_.map_->size() == other.value_.map_->size()
             && (*value_.map_) == (*other.value_.map_);
#else
   case arrayValue:
      return value_.array_->compare( *(other.value_.array_) ) == 0;
   case objectValue:
      return value_.map_->compare( *(other.value_.map_) ) == 0;
#endif
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return false;  // unreachable
}

bool 
Value::operator !=( const Value &other ) const
{
   return !( *this == other );
}

const wchar_t *
Value::asCString() const
{
   JSON_ASSERT( type_ == stringValue );
   return value_.string_;
}


std::wstring 
Value::asString() const
{
   switch ( type_ )
   {
   case nullValue:
      return L"";
   case stringValue:
      return value_.string_ ? value_.string_ : L"";
   case booleanValue:
      return value_.bool_ ? L"true" : L"false";
   case intValue:
   case uintValue:
   case realValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to string" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return L""; // unreachable
}

# ifdef JSON_USE_CPPTL
CppTL::Constwstring 
Value::asConstString() const
{
   return CppTL::ConstString( asString().c_str() );
}
# endif


Value::Int 
Value::asInt() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0;
   case intValue:
      JSON_ASSERT_MESSAGE( value_.int_ >= minInt  &&  value_.int_ <= maxInt, "unsigned integer out of signed int range" );
      return Int(value_.int_);
   case uintValue:
      JSON_ASSERT_MESSAGE( value_.uint_ <= UInt(maxInt), "unsigned integer out of signed int range" );
      return Int(value_.uint_);
   case realValue:
      JSON_ASSERT_MESSAGE( value_.real_ >= minInt  &&  value_.real_ <= maxInt, "Real out of signed integer range" );
      return Int( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1 : 0;
   case stringValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to int" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}


Value::UInt 
Value::asUInt() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0;
   case intValue:
      JSON_ASSERT_MESSAGE( value_.int_ >= 0, "Negative integer can not be converted to unsigned integer" );
      JSON_ASSERT_MESSAGE( value_.int_ <= maxUInt, "signed integer out of UInt range" );
      return UInt(value_.int_);
   case uintValue:
      JSON_ASSERT_MESSAGE( value_.uint_ <= maxUInt, "unsigned integer out of UInt range" );
      return UInt(value_.uint_);
   case realValue:
      JSON_ASSERT_MESSAGE( value_.real_ >= 0  &&  value_.real_ <= maxUInt,  "Real out of unsigned integer range" );
      return UInt( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1 : 0;
   case stringValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to uint" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}


# if defined(JSON_HAS_INT64)

Value::Int64
Value::asInt64() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0;
   case intValue:
      return value_.int_;
   case uintValue:
      JSON_ASSERT_MESSAGE( value_.uint_ <= UInt64(maxInt64), "unsigned integer out of Int64 range" );
      return value_.uint_;
   case realValue:
      JSON_ASSERT_MESSAGE( value_.real_ >= minInt64  &&  value_.real_ <= maxInt64, "Real out of Int64 range" );
      return Int( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1 : 0;
   case stringValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to Int64" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}


Value::UInt64
Value::asUInt64() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0;
   case intValue:
      JSON_ASSERT_MESSAGE( value_.int_ >= 0, "Negative integer can not be converted to UInt64" );
      return value_.int_;
   case uintValue:
      return value_.uint_;
   case realValue:
      JSON_ASSERT_MESSAGE( value_.real_ >= 0  &&  value_.real_ <= maxUInt64,  "Real out of UInt64 range" );
      return UInt( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1 : 0;
   case stringValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to UInt64" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}
# endif // if defined(JSON_HAS_INT64)


LargestInt 
Value::asLargestInt() const
{
#if defined(JSON_NO_INT64)
    return asInt();
#else
    return asInt64();
#endif
}


LargestUInt 
Value::asLargestUInt() const
{
#if defined(JSON_NO_INT64)
    return asUInt();
#else
    return asUInt64();
#endif
}


double 
Value::asDouble() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0.0;
   case intValue:
      return static_cast<double>( value_.int_ );
   case uintValue:
#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
      return static_cast<double>( value_.uint_ );
#else // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
      return static_cast<double>( Int(value_.uint_/2) ) * 2 + Int(value_.uint_ & 1);
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
   case realValue:
      return value_.real_;
   case booleanValue:
      return value_.bool_ ? 1.0 : 0.0;
   case stringValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to double" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}

float
Value::asFloat() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0.0f;
   case intValue:
      return static_cast<float>( value_.int_ );
   case uintValue:
#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
      return static_cast<float>( value_.uint_ );
#else // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
      return static_cast<float>( Int(value_.uint_/2) ) * 2 + Int(value_.uint_ & 1);
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
   case realValue:
      return static_cast<float>( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1.0f : 0.0f;
   case stringValue:
   case arrayValue:
   case objectValue:
      JSON_FAIL_MESSAGE( "Type is not convertible to float" );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0.0f; // unreachable;
}

bool 
Value::asBool() const
{
   switch ( type_ )
   {
   case nullValue:
      return false;
   case intValue:
   case uintValue:
      return value_.int_ != 0;
   case realValue:
      return value_.real_ != 0.0;
   case booleanValue:
      return value_.bool_;
   case stringValue:
      return value_.string_  &&  value_.string_[0] != 0;
   case arrayValue:
   case objectValue:
      return value_.map_->size() != 0;
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return false; // unreachable;
}


bool 
Value::isConvertibleTo( ValueType other ) const
{
   switch ( type_ )
   {
   case nullValue:
      return true;
   case intValue:
      return ( other == nullValue  &&  value_.int_ == 0 )
             || other == intValue
             || ( other == uintValue  && value_.int_ >= 0 )
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case uintValue:
      return ( other == nullValue  &&  value_.uint_ == 0 )
             || ( other == intValue  && value_.uint_ <= (unsigned)maxInt )
             || other == uintValue
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case realValue:
      return ( other == nullValue  &&  value_.real_ == 0.0 )
             || ( other == intValue  &&  value_.real_ >= minInt  &&  value_.real_ <= maxInt )
             || ( other == uintValue  &&  value_.real_ >= 0  &&  value_.real_ <= maxUInt )
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case booleanValue:
      return ( other == nullValue  &&  value_.bool_ == false )
             || other == intValue
             || other == uintValue
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case stringValue:
      return other == stringValue
             || ( other == nullValue  &&  (!value_.string_  ||  value_.string_[0] == 0) );
   case arrayValue:
      return other == arrayValue
             ||  ( other == nullValue  &&  value_.map_->size() == 0 );
   case objectValue:
      return other == objectValue
             ||  ( other == nullValue  &&  value_.map_->size() == 0 );
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return false; // unreachable;
}


/// Number of values in array or object
ArrayIndex 
Value::size() const
{
   switch ( type_ )
   {
   case nullValue:
   case intValue:
   case uintValue:
   case realValue:
   case booleanValue:
   case stringValue:
      return 0;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:  // size of the array is highest index + 1
      if ( !value_.map_->empty() )
      {
         ObjectValues::const_iterator itLast = value_.map_->end();
         --itLast;
         return (*itLast).first.index()+1;
      }
      return 0;
   case objectValue:
      return ArrayIndex( value_.map_->size() );
#else
   case arrayValue:
      return Int( value_.array_->size() );
   case objectValue:
      return Int( value_.map_->size() );
#endif
   default:
      JSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}


bool 
Value::empty() const
{
   if ( isNull() || isArray() || isObject() )
      return size() == 0u;
   else
      return false;
}


bool
Value::operator!() const
{
   return isNull();
}


void 
Value::clear()
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue  || type_ == objectValue );

   switch ( type_ )
   {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      value_.map_->clear();
      break;
#else
   case arrayValue:
      value_.array_->clear();
      break;
   case objectValue:
      value_.map_->clear();
      break;
#endif
   default:
      break;
   }
}

void 
Value::resize( ArrayIndex newSize )
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue );
   if ( type_ == nullValue )
      *this = Value( arrayValue );
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   ArrayIndex oldSize = size();
   if ( newSize == 0 )
      clear();
   else if ( newSize > oldSize )
      (*this)[ newSize - 1 ];
   else
   {
      for ( ArrayIndex index = newSize; index < oldSize; ++index )
      {
         value_.map_->erase( index );
      }
      assert( size() == newSize );
   }
#else
   value_.array_->resize( newSize );
#endif
}


Value &
Value::operator[]( ArrayIndex index )
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue );
   if ( type_ == nullValue )
      *this = Value( arrayValue );
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   CZString key( index );
   ObjectValues::iterator it = value_.map_->lower_bound( key );
   if ( it != value_.map_->end()  &&  (*it).first == key )
      return (*it).second;

   ObjectValues::value_type defaultValue( key, null );
   it = value_.map_->insert( it, defaultValue );
   return (*it).second;
#else
   return value_.array_->resolveReference( index );
#endif
}


Value &
Value::operator[]( int index )
{
   JSON_ASSERT( index >= 0 );
   return (*this)[ ArrayIndex(index) ];
}


const Value &
Value::operator[]( ArrayIndex index ) const
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue );
   if ( type_ == nullValue )
      return null;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   CZString key( index );
   ObjectValues::const_iterator it = value_.map_->find( key );
   if ( it == value_.map_->end() )
      return null;
   return (*it).second;
#else
   Value *value = value_.array_->find( index );
   return value ? *value : null;
#endif
}


const Value &
Value::operator[]( int index ) const
{
   JSON_ASSERT( index >= 0 );
   return (*this)[ ArrayIndex(index) ];
}


Value &
Value::operator[]( const wchar_t *key )
{
   return resolveReference( key, false );
}


Value &
Value::resolveReference( const wchar_t *key, 
                         bool isStatic )
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
      *this = Value( objectValue );
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   CZString actualKey( key, isStatic ? CZString::noDuplication 
                                     : CZString::duplicateOnCopy );
   ObjectValues::iterator it = value_.map_->lower_bound( actualKey );
   if ( it != value_.map_->end()  &&  (*it).first == actualKey )
      return (*it).second;

   ObjectValues::value_type defaultValue( actualKey, null );
   it = value_.map_->insert( it, defaultValue );
   Value &value = (*it).second;
   return value;
#else
   return value_.map_->resolveReference( key, isStatic );
#endif
}


Value 
Value::get( ArrayIndex index, 
            const Value &defaultValue ) const
{
   const Value *value = &((*this)[index]);
   return value == &null ? defaultValue : *value;
}


bool 
Value::isValidIndex( ArrayIndex index ) const
{
   return index < size();
}



const Value &
Value::operator[]( const wchar_t *key ) const
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
      return null;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   CZString actualKey( key, CZString::noDuplication );
   ObjectValues::const_iterator it = value_.map_->find( actualKey );
   if ( it == value_.map_->end() )
      return null;
   return (*it).second;
#else
   const Value *value = value_.map_->find( key );
   return value ? *value : null;
#endif
}


Value &
Value::operator[]( const std::wstring &key )
{
   return (*this)[ key.c_str() ];
}


const Value &
Value::operator[]( const std::wstring &key ) const
{
   return (*this)[ key.c_str() ];
}

Value &
Value::operator[]( const StaticString &key )
{
   return resolveReference( key, true );
}


# ifdef JSON_USE_CPPTL
Value &
Value::operator[]( const CppTL::Constwstring &key )
{
   return (*this)[ key.c_str() ];
}


const Value &
Value::operator[]( const CppTL::Constwstring &key ) const
{
   return (*this)[ key.c_str() ];
}
# endif


Value &
Value::append( const Value &value )
{
   return (*this)[size()] = value;
}


Value 
Value::get( const wchar_t *key, 
            const Value &defaultValue ) const
{
   const Value *value = &((*this)[key]);
   return value == &null ? defaultValue : *value;
}


Value 
Value::get( const std::wstring &key,
            const Value &defaultValue ) const
{
   return get( key.c_str(), defaultValue );
}

Value
Value::removeMember( const wchar_t* key )
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
      return null;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   CZString actualKey( key, CZString::noDuplication );
   ObjectValues::iterator it = value_.map_->find( actualKey );
   if ( it == value_.map_->end() )
      return null;
   Value old(it->second);
   value_.map_->erase(it);
   return old;
#else
   Value *value = value_.map_->find( key );
   if (value){
      Value old(*value);
      value_.map_.remove( key );
      return old;
   } else {
      return null;
   }
#endif
}

Value
Value::removeMember( const std::wstring &key )
{
   return removeMember( key.c_str() );
}

# ifdef JSON_USE_CPPTL
Value 
Value::get( const CppTL::Constwstring &key,
            const Value &defaultValue ) const
{
   return get( key.c_str(), defaultValue );
}
# endif

bool 
Value::isMember( const wchar_t *key ) const
{
   const Value *value = &((*this)[key]);
   return value != &null;
}


bool 
Value::isMember( const std::wstring &key ) const
{
   return isMember( key.c_str() );
}


# ifdef JSON_USE_CPPTL
bool 
Value::isMember( const CppTL::Constwstring &key ) const
{
   return isMember( key.c_str() );
}
#endif

Value::Members 
Value::getMemberNames() const
{
   JSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
       return Value::Members();
   Members members;
   members.reserve( value_.map_->size() );
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   ObjectValues::const_iterator it = value_.map_->begin();
   ObjectValues::const_iterator itEnd = value_.map_->end();
   for ( ; it != itEnd; ++it )
      members.push_back( std::wstring( (*it).first.c_str() ) );
#else
   ValueInternalMap::IteratorState it;
   ValueInternalMap::IteratorState itEnd;
   value_.map_->makeBeginIterator( it );
   value_.map_->makeEndIterator( itEnd );
   for ( ; !ValueInternalMap::equals( it, itEnd ); ValueInternalMap::increment(it) )
      members.push_back( std::wstring( ValueInternalMap::key( it ) ) );
#endif
   return members;
}
//
//# ifdef JSON_USE_CPPTL
//EnumMemberNames
//Value::enumMemberNames() const
//{
//   if ( type_ == objectValue )
//   {
//      return CppTL::Enum::any(  CppTL::Enum::transform(
//         CppTL::Enum::keys( *(value_.map_), CppTL::Type<const CZString &>() ),
//         MemberNamesTransform() ) );
//   }
//   return EnumMemberNames();
//}
//
//
//EnumValues 
//Value::enumValues() const
//{
//   if ( type_ == objectValue  ||  type_ == arrayValue )
//      return CppTL::Enum::anyValues( *(value_.map_), 
//                                     CppTL::Type<const Value &>() );
//   return EnumValues();
//}
//
//# endif


bool
Value::isNull() const
{
   return type_ == nullValue;
}


bool 
Value::isBool() const
{
   return type_ == booleanValue;
}


bool 
Value::isInt() const
{
   return type_ == intValue;
}


bool 
Value::isUInt() const
{
   return type_ == uintValue;
}


bool 
Value::isIntegral() const
{
   return type_ == intValue  
          ||  type_ == uintValue  
          ||  type_ == booleanValue;
}


bool 
Value::isDouble() const
{
   return type_ == realValue;
}


bool 
Value::isNumeric() const
{
   return isIntegral() || isDouble();
}


bool 
Value::isString() const
{
   return type_ == stringValue;
}


bool 
Value::isArray() const
{
   return type_ == nullValue  ||  type_ == arrayValue;
}


bool 
Value::isObject() const
{
   return type_ == nullValue  ||  type_ == objectValue;
}


void 
Value::setComment( const wchar_t *comment,
                   CommentPlacement placement )
{
   if ( !comments_ )
      comments_ = new CommentInfo[numberOfCommentPlacement];
   comments_[placement].setComment( comment );
}


void 
Value::setComment( const std::wstring &comment,
                   CommentPlacement placement )
{
   setComment( comment.c_str(), placement );
}


bool 
Value::hasComment( CommentPlacement placement ) const
{
   return comments_ != 0  &&  comments_[placement].comment_ != 0;
}

std::wstring 
Value::getComment( CommentPlacement placement ) const
{
   if ( hasComment(placement) )
      return comments_[placement].comment_;
   return L"";
}


std::wstring 
Value::toStyledString() const
{
   StyledWriter writer;
   return writer.write( *this );
}


Value::const_iterator 
Value::begin() const
{
   switch ( type_ )
   {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeBeginIterator( it );
         return const_iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeBeginIterator( it );
         return const_iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return const_iterator( value_.map_->begin() );
      break;
#endif
   default:
      break;
   }
   return const_iterator();
}

Value::const_iterator 
Value::end() const
{
   switch ( type_ )
   {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeEndIterator( it );
         return const_iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeEndIterator( it );
         return const_iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return const_iterator( value_.map_->end() );
      break;
#endif
   default:
      break;
   }
   return const_iterator();
}


Value::iterator 
Value::begin()
{
   switch ( type_ )
   {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeBeginIterator( it );
         return iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeBeginIterator( it );
         return iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return iterator( value_.map_->begin() );
      break;
#endif
   default:
      break;
   }
   return iterator();
}

Value::iterator 
Value::end()
{
   switch ( type_ )
   {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeEndIterator( it );
         return iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeEndIterator( it );
         return iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return iterator( value_.map_->end() );
      break;
#endif
   default:
      break;
   }
   return iterator();
}


// class PathArgument
// //////////////////////////////////////////////////////////////////

PathArgument::PathArgument()
   : kind_( kindNone )
{
}


PathArgument::PathArgument( ArrayIndex index )
   : index_( index )
   , kind_( kindIndex )
{
}


PathArgument::PathArgument( const wchar_t *key )
   : key_( key )
   , kind_( kindKey )
{
}


PathArgument::PathArgument( const std::wstring &key )
   : key_( key.c_str() )
   , kind_( kindKey )
{
}

// class Path
// //////////////////////////////////////////////////////////////////

Path::Path( const std::wstring &path,
            const PathArgument &a1,
            const PathArgument &a2,
            const PathArgument &a3,
            const PathArgument &a4,
            const PathArgument &a5 )
{
   InArgs in;
   in.push_back( &a1 );
   in.push_back( &a2 );
   in.push_back( &a3 );
   in.push_back( &a4 );
   in.push_back( &a5 );
   makePath( path, in );
}


void 
Path::makePath( const std::wstring &path,
                const InArgs &in )
{
   const wchar_t *current = path.c_str();
   const wchar_t *end = current + path.length();
   InArgs::const_iterator itInArg = in.begin();
   while ( current != end )
   {
      if ( *current == L'[' )
      {
         ++current;
         if ( *current == L'%' )
            addPathInArg( path, in, itInArg, PathArgument::kindIndex );
         else
         {
            ArrayIndex index = 0;
            for ( ; current != end && *current >= L'0'  &&  *current <= L'9'; ++current )
               index = index * 10 + ArrayIndex(*current - L'0');
            args_.push_back( index );
         }
         if ( current == end  ||  *current++ != L']' )
            invalidPath( path, int(current - path.c_str()) );
      }
      else if ( *current == L'%' )
      {
         addPathInArg( path, in, itInArg, PathArgument::kindKey );
         ++current;
      }
      else if ( *current == L'.' )
      {
         ++current;
      }
      else
      {
         const wchar_t *beginName = current;
         while ( current != end  &&  !wcschr( L"[.", *current ) )
            ++current;
         args_.push_back( std::wstring( beginName, current ) );
      }
   }
}


void 
Path::addPathInArg( const std::wstring &path, 
                    const InArgs &in, 
                    InArgs::const_iterator &itInArg, 
                    PathArgument::Kind kind )
{
   if ( itInArg == in.end() )
   {
      // Error: missing argument %d
   }
   else if ( (*itInArg)->kind_ != kind )
   {
      // Error: bad argument type
   }
   else
   {
      args_.push_back( **itInArg );
   }
}


void 
Path::invalidPath( const std::wstring &path, 
                   int location )
{
   // Error: invalid path.
}


const Value &
Path::resolve( const Value &root ) const
{
   const Value *node = &root;
   for ( Args::const_iterator it = args_.begin(); it != args_.end(); ++it )
   {
      const PathArgument &arg = *it;
      if ( arg.kind_ == PathArgument::kindIndex )
      {
         if ( !node->isArray()  ||  node->isValidIndex( arg.index_ ) )
         {
            // Error: unable to resolve path (array value expected at position...
         }
         node = &((*node)[arg.index_]);
      }
      else if ( arg.kind_ == PathArgument::kindKey )
      {
         if ( !node->isObject() )
         {
            // Error: unable to resolve path (object value expected at position...)
         }
         node = &((*node)[arg.key_]);
         if ( node == &Value::null )
         {
            // Error: unable to resolve path (object has no member named L'' at position...)
         }
      }
   }
   return *node;
}


Value 
Path::resolve( const Value &root, 
               const Value &defaultValue ) const
{
   const Value *node = &root;
   for ( Args::const_iterator it = args_.begin(); it != args_.end(); ++it )
   {
      const PathArgument &arg = *it;
      if ( arg.kind_ == PathArgument::kindIndex )
      {
         if ( !node->isArray()  ||  node->isValidIndex( arg.index_ ) )
            return defaultValue;
         node = &((*node)[arg.index_]);
      }
      else if ( arg.kind_ == PathArgument::kindKey )
      {
         if ( !node->isObject() )
            return defaultValue;
         node = &((*node)[arg.key_]);
         if ( node == &Value::null )
            return defaultValue;
      }
   }
   return *node;
}


Value &
Path::make( Value &root ) const
{
   Value *node = &root;
   for ( Args::const_iterator it = args_.begin(); it != args_.end(); ++it )
   {
      const PathArgument &arg = *it;
      if ( arg.kind_ == PathArgument::kindIndex )
      {
         if ( !node->isArray() )
         {
            // Error: node is not an array at position ...
         }
         node = &((*node)[arg.index_]);
      }
      else if ( arg.kind_ == PathArgument::kindKey )
      {
         if ( !node->isObject() )
         {
            // Error: node is not an object at position...
         }
         node = &((*node)[arg.key_]);
      }
   }
   return *node;
}


} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_value.cpp
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_writer.cpp
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
# include <json/writer.h>
# include L"json_tool.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <utility>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#if _MSC_VER >= 1400 // VC++ 8.0
#pragma warning( disable : 4996 )   // disable warning about strdup being deprecated.
#endif

namespace Json {

static bool containsControlCharacter( const wchar_t* str )
{
   while ( *str ) 
   {
      if ( isControlCharacter( *(str++) ) )
         return true;
   }
   return false;
}


std::wstring valueToString( LargestInt value )
{
   UIntToStringBuffer buffer;
   wchar_t *current = buffer + sizeof(buffer);
   bool isNegative = value < 0;
   if ( isNegative )
      value = -value;
   uintToString( LargestUInt(value), current );
   if ( isNegative )
      *--current = L'-';
   assert( current >= buffer );
   return current;
}


std::wstring valueToString( LargestUInt value )
{
   UIntToStringBuffer buffer;
   wchar_t *current = buffer + sizeof(buffer);
   uintToString( value, current );
   assert( current >= buffer );
   return current;
}

#if defined(JSON_HAS_INT64)

std::wstring valueToString( Int value )
{
   return valueToString( LargestInt(value) );
}


std::wstring valueToString( UInt value )
{
   return valueToString( LargestUInt(value) );
}

#endif // # if defined(JSON_HAS_INT64)


std::wstring valueToString( double value )
{
   wchar_t buffer[32];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__) // Use secure version with visual studio 2005 to avoid warning. 
   swprintf_s(buffer, sizeof(buffer), L"%#.16g", value); 
#else	
   swprintf(buffer, L"%#.16g", value); 
#endif
   wchar_t* ch = buffer + wcslen(buffer) - 1;
   if (*ch != L'0') return buffer; // nothing to truncate, so save time
   while(ch > buffer && *ch == L'0'){
     --ch;
   }
   wchar_t* last_nonzero = ch;
   while(ch >= buffer){
     switch(*ch){
     case L'0':
     case L'1':
     case L'2':
     case L'3':
     case L'4':
     case L'5':
     case L'6':
     case L'7':
     case L'8':
     case L'9':
       --ch;
       continue;
     case L'.':
       // Truncate zeroes to save bytes in output, but keep one.
       *(last_nonzero+2) = L'\0';
       return buffer;
     default:
       return buffer;
     }
   }
   return buffer;
}


std::wstring valueToString( bool value )
{
   return value ? L"true" : L"false";
}

std::wstring valueToQuotedString( const wchar_t *value )
{
   // Not sure how to handle unicode...
   if (wcspbrk(value, L"\"\\\b\f\n\r\t") == NULL && !containsControlCharacter( value ))
      return std::wstring(L"\"") + value + L"\"";
   // We have to walk value and escape any special characters.
   // Appending to std::wstring is not efficient, but this should be rare.
   // (Note: forward slashes are *not* rare, but I am not escaping them.)
   std::wstring::size_type maxsize = wcslen(value)*2 + 3; // allescaped+quotes+NULL
   std::wstring result;
   result.reserve(maxsize); // to avoid lots of mallocs
   result += L"\"";
   for (const wchar_t* c=value; *c != 0; ++c)
   {
      switch(*c)
      {
         case L'\"':
            result += L"\\\"";
            break;
         case L'\\':
            result += L"\\\\";
            break;
         case L'\b':
            result += L"\\b";
            break;
         case L'\f':
            result += L"\\f";
            break;
         case L'\n':
            result += L"\\n";
            break;
         case L'\r':
            result += L"\\r";
            break;
         case L'\t':
            result += L"\\t";
            break;
         //case L'/':
            // Even though \/ is considered a legal escape in JSON, a bare
            // slash is also legal, so I see no reason to escape it.
            // (I hope I am not misunderstanding something.
            // blep notes: actually escaping \/ may be useful in javascript to avoid </ 
            // sequence.
            // Should add a flag to allow this compatibility mode and prevent this 
            // sequence from occurring.
         default:
            if ( isControlCharacter( *c ) )
            {
               std::wostringstream oss;
               oss << L"\\u" << std::hex << std::uppercase << std::setfill(L'0') << std::setw(4) << static_cast<int>(*c);
               result += oss.str();
            }
            else
            {
               result += *c;
            }
            break;
      }
   }
   result += L"\"";
   return result;
}

// Class Writer
// //////////////////////////////////////////////////////////////////
Writer::~Writer()
{
}


// Class FastWriter
// //////////////////////////////////////////////////////////////////

FastWriter::FastWriter()
   : yamlCompatiblityEnabled_( false )
{
}


void 
FastWriter::enableYAMLCompatibility()
{
   yamlCompatiblityEnabled_ = true;
}


std::wstring 
FastWriter::write( const Value &root )
{
   document_ = L"";
   writeValue( root );
   document_ += L"\n";
   return document_;
}


void 
FastWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      document_ += L"null";
      break;
   case intValue:
      document_ += valueToString( value.asLargestInt() );
      break;
   case uintValue:
      document_ += valueToString( value.asLargestUInt() );
      break;
   case realValue:
      document_ += valueToString( value.asDouble() );
      break;
   case stringValue:
      document_ += valueToQuotedString( value.asCString() );
      break;
   case booleanValue:
      document_ += valueToString( value.asBool() );
      break;
   case arrayValue:
      {
         document_ += L"[";
         int size = value.size();
         for ( int index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += L",";
            writeValue( value[index] );
         }
         document_ += L"]";
      }
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         document_ += L"{";
         for ( Value::Members::iterator it = members.begin(); 
               it != members.end(); 
               ++it )
         {
            const std::wstring &name = *it;
            if ( it != members.begin() )
               document_ += L",";
            document_ += valueToQuotedString( name.c_str() );
            document_ += yamlCompatiblityEnabled_ ? L": L" 
                                                  : L":";
            writeValue( value[name] );
         }
         document_ += L"}";
      }
      break;
   }
}


// Class StyledWriter
// //////////////////////////////////////////////////////////////////

StyledWriter::StyledWriter()
   : rightMargin_( 74 )
   , indentSize_( 3 )
{
}


std::wstring 
StyledWriter::write( const Value &root )
{
   document_ = L"";
   addChildValues_ = false;
   indentString_ = L"";
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   document_ += L"\n";
   return document_;
}


void 
StyledWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      pushValue( L"null" );
      break;
   case intValue:
      pushValue( valueToString( value.asLargestInt() ) );
      break;
   case uintValue:
      pushValue( valueToString( value.asLargestUInt() ) );
      break;
   case realValue:
      pushValue( valueToString( value.asDouble() ) );
      break;
   case stringValue:
      pushValue( valueToQuotedString( value.asCString() ) );
      break;
   case booleanValue:
      pushValue( valueToString( value.asBool() ) );
      break;
   case arrayValue:
      writeArrayValue( value);
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         if ( members.empty() )
            pushValue( L"{}" );
         else
         {
            writeWithIndent( L"{" );
            indent();
            Value::Members::iterator it = members.begin();
            for (;;)
            {
               const std::wstring &name = *it;
               const Value &childValue = value[name];
               writeCommentBeforeValue( childValue );
               writeWithIndent( valueToQuotedString( name.c_str() ) );
               document_ += L" : L";
               writeValue( childValue );
               if ( ++it == members.end() )
               {
                  writeCommentAfterValueOnSameLine( childValue );
                  break;
               }
               document_ += L",";
               writeCommentAfterValueOnSameLine( childValue );
            }
            unindent();
            writeWithIndent( L"}" );
         }
      }
      break;
   }
}


void 
StyledWriter::writeArrayValue( const Value &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      pushValue( L"[]" );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( L"[" );
         indent();
         bool hasChildValue = !childValues_.empty();
         unsigned index =0;
         for (;;)
         {
            const Value &childValue = value[index];
            writeCommentBeforeValue( childValue );
            if ( hasChildValue )
               writeWithIndent( childValues_[index] );
            else
            {
               writeIndent();
               writeValue( childValue );
            }
            if ( ++index == size )
            {
               writeCommentAfterValueOnSameLine( childValue );
               break;
            }
            document_ += L",";
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( L"]" );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         document_ += L"[ L";
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += L", L";
            document_ += childValues_[index];
         }
         document_ += L" ]";
      }
   }
}


bool 
StyledWriter::isMultineArray( const Value &value )
{
   int size = value.size();
   bool isMultiLine = size*3 >= rightMargin_ ;
   childValues_.clear();
   for ( int index =0; index < size  &&  !isMultiLine; ++index )
   {
      const Value &childValue = value[index];
      isMultiLine = isMultiLine  ||
                     ( (childValue.isArray()  ||  childValue.isObject())  &&  
                        childValue.size() > 0 );
   }
   if ( !isMultiLine ) // check if line length > max line length
   {
      childValues_.reserve( size );
      addChildValues_ = true;
      int lineLength = 4 + (size-1)*2; // L'[ L' + L', L'*n + L' ]'
      for ( int index =0; index < size  &&  !isMultiLine; ++index )
      {
         writeValue( value[index] );
         lineLength += int( childValues_[index].length() );
         isMultiLine = isMultiLine  &&  hasCommentForValue( value[index] );
      }
      addChildValues_ = false;
      isMultiLine = isMultiLine  ||  lineLength >= rightMargin_;
   }
   return isMultiLine;
}


void 
StyledWriter::pushValue( const std::wstring &value )
{
   if ( addChildValues_ )
      childValues_.push_back( value );
   else
      document_ += value;
}


void 
StyledWriter::writeIndent()
{
   if ( !document_.empty() )
   {
      wchar_t last = document_[document_.length()-1];
      if ( last == L' ' )     // already indented
         return;
      if ( last != L'\n' )    // Comments may add new-line
         document_ += L'\n';
   }
   document_ += indentString_;
}


void 
StyledWriter::writeWithIndent( const std::wstring &value )
{
   writeIndent();
   document_ += value;
}


void 
StyledWriter::indent()
{
   indentString_ += std::wstring( indentSize_, L' ' );
}


void 
StyledWriter::unindent()
{
   assert( int(indentString_.size()) >= indentSize_ );
   indentString_.resize( indentString_.size() - indentSize_ );
}


void 
StyledWriter::writeCommentBeforeValue( const Value &root )
{
   if ( !root.hasComment( commentBefore ) )
      return;
   document_ += normalizeEOL( root.getComment( commentBefore ) );
   document_ += L"\n";
}


void 
StyledWriter::writeCommentAfterValueOnSameLine( const Value &root )
{
   if ( root.hasComment( commentAfterOnSameLine ) )
      document_ += L" L" + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      document_ += L"\n";
      document_ += normalizeEOL( root.getComment( commentAfter ) );
      document_ += L"\n";
   }
}


bool 
StyledWriter::hasCommentForValue( const Value &value )
{
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
}


std::wstring 
StyledWriter::normalizeEOL( const std::wstring &text )
{
   std::wstring normalized;
   normalized.reserve( text.length() );
   const wchar_t *begin = text.c_str();
   const wchar_t *end = begin + text.length();
   const wchar_t *current = begin;
   while ( current != end )
   {
      wchar_t c = *current++;
      if ( c == L'\r' ) // mac or dos EOL
      {
         if ( *current == L'\n' ) // convert dos EOL
            ++current;
         normalized += L'\n';
      }
      else // handle unix EOL & other char
         normalized += c;
   }
   return normalized;
}


// Class StyledStreamWriter
// //////////////////////////////////////////////////////////////////

StyledStreamWriter::StyledStreamWriter( std::wstring indentation )
   : document_(NULL)
   , rightMargin_( 74 )
   , indentation_( indentation )
{
}


void
StyledStreamWriter::write( std::ostream &out, const Value &root )
{
   document_ = &out;
   addChildValues_ = false;
   indentString_ = L"";
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   *document_ << L"\n";
   document_ = NULL; // Forget the stream, for safety.
}


void 
StyledStreamWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      pushValue( L"null" );
      break;
   case intValue:
      pushValue( valueToString( value.asLargestInt() ) );
      break;
   case uintValue:
      pushValue( valueToString( value.asLargestUInt() ) );
      break;
   case realValue:
      pushValue( valueToString( value.asDouble() ) );
      break;
   case stringValue:
      pushValue( valueToQuotedString( value.asCString() ) );
      break;
   case booleanValue:
      pushValue( valueToString( value.asBool() ) );
      break;
   case arrayValue:
      writeArrayValue( value);
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         if ( members.empty() )
            pushValue( L"{}" );
         else
         {
            writeWithIndent( L"{" );
            indent();
            Value::Members::iterator it = members.begin();
            for (;;)
            {
               const std::wstring &name = *it;
               const Value &childValue = value[name];
               writeCommentBeforeValue( childValue );
               writeWithIndent( valueToQuotedString( name.c_str() ) );
               *document_ << L" : L";
               writeValue( childValue );
               if ( ++it == members.end() )
               {
                  writeCommentAfterValueOnSameLine( childValue );
                  break;
               }
               *document_ << L",";
               writeCommentAfterValueOnSameLine( childValue );
            }
            unindent();
            writeWithIndent( L"}" );
         }
      }
      break;
   }
}


void 
StyledStreamWriter::writeArrayValue( const Value &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      pushValue( L"[]" );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( L"[" );
         indent();
         bool hasChildValue = !childValues_.empty();
         unsigned index =0;
         for (;;)
         {
            const Value &childValue = value[index];
            writeCommentBeforeValue( childValue );
            if ( hasChildValue )
               writeWithIndent( childValues_[index] );
            else
            {
               writeIndent();
               writeValue( childValue );
            }
            if ( ++index == size )
            {
               writeCommentAfterValueOnSameLine( childValue );
               break;
            }
            *document_ << L",";
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( L"]" );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         *document_ << L"[ L";
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               *document_ << L", L";
            *document_ << childValues_[index];
         }
         *document_ << L" ]";
      }
   }
}


bool 
StyledStreamWriter::isMultineArray( const Value &value )
{
   int size = value.size();
   bool isMultiLine = size*3 >= rightMargin_ ;
   childValues_.clear();
   for ( int index =0; index < size  &&  !isMultiLine; ++index )
   {
      const Value &childValue = value[index];
      isMultiLine = isMultiLine  ||
                     ( (childValue.isArray()  ||  childValue.isObject())  &&  
                        childValue.size() > 0 );
   }
   if ( !isMultiLine ) // check if line length > max line length
   {
      childValues_.reserve( size );
      addChildValues_ = true;
      int lineLength = 4 + (size-1)*2; // L'[ L' + L', L'*n + L' ]'
      for ( int index =0; index < size  &&  !isMultiLine; ++index )
      {
         writeValue( value[index] );
         lineLength += int( childValues_[index].length() );
         isMultiLine = isMultiLine  &&  hasCommentForValue( value[index] );
      }
      addChildValues_ = false;
      isMultiLine = isMultiLine  ||  lineLength >= rightMargin_;
   }
   return isMultiLine;
}


void 
StyledStreamWriter::pushValue( const std::wstring &value )
{
   if ( addChildValues_ )
      childValues_.push_back( value );
   else
      *document_ << value;
}


void 
StyledStreamWriter::writeIndent()
{
  /*
    Some comments in this method would have been nice. ;-)

   if ( !document_.empty() )
   {
      wchar_t last = document_[document_.length()-1];
      if ( last == L' L' )     // already indented
         return;
      if ( last != L'\n' )    // Comments may add new-line
         *document_ << L'\n';
   }
  */
   *document_ << L'\n' << indentString_;
}


void 
StyledStreamWriter::writeWithIndent( const std::wstring &value )
{
   writeIndent();
   *document_ << value;
}


void 
StyledStreamWriter::indent()
{
   indentString_ += indentation_;
}


void 
StyledStreamWriter::unindent()
{
   assert( indentString_.size() >= indentation_.size() );
   indentString_.resize( indentString_.size() - indentation_.size() );
}


void 
StyledStreamWriter::writeCommentBeforeValue( const Value &root )
{
   if ( !root.hasComment( commentBefore ) )
      return;
   *document_ << normalizeEOL( root.getComment( commentBefore ) );
   *document_ << L"\n";
}


void 
StyledStreamWriter::writeCommentAfterValueOnSameLine( const Value &root )
{
   if ( root.hasComment( commentAfterOnSameLine ) )
      *document_ << L" L" + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      *document_ << L"\n";
      *document_ << normalizeEOL( root.getComment( commentAfter ) );
      *document_ << L"\n";
   }
}


bool 
StyledStreamWriter::hasCommentForValue( const Value &value )
{
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
}


std::wstring 
StyledStreamWriter::normalizeEOL( const std::wstring &text )
{
   std::wstring normalized;
   normalized.reserve( text.length() );
   const wchar_t *begin = text.c_str();
   const wchar_t *end = begin + text.length();
   const wchar_t *current = begin;
   while ( current != end )
   {
      wchar_t c = *current++;
      if ( c == L'\r' ) // mac or dos EOL
      {
         if ( *current == L'\n' ) // convert dos EOL
            ++current;
         normalized += L'\n';
      }
      else // handle unix EOL & other char
         normalized += c;
   }
   return normalized;
}


std::ostream& operator<<( std::ostream &sout, const Value &root )
{
   Json::StyledStreamWriter writer;
   writer.write(sout, root);
   return sout;
}


} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_writer.cpp
// //////////////////////////////////////////////////////////////////////





