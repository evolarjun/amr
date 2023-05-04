// common.cpp

/*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* Author: Vyacheslav Brover
*
* File Description:
*   Common utilities
*
*/


#undef NDEBUG
#include "common.inc"

#include "common.hpp"

#include <sstream>
#include <cstring>
#include <regex>
#include <csignal>  

#ifndef _MSC_VER
  #include <execinfo.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
  #ifdef __APPLE__
    #include <sys/sysctl.h>
  #endif
#endif




namespace Common_sp
{
 


vector<string> programArgs;
string programName;



string getCommandLine ()
{
  string commandLine;
  for (const string& s : programArgs)
  {
    const bool bad =    s. empty () 
                     || contains (s, ' ')
                     || contains (s, '|')
                     || contains (s, ';')
                     || contains (s, '#')
                     || contains (s, '*')
                     || contains (s, '?')
                     || contains (s, '$')
                     || contains (s, '(')
                     || contains (s, ')')
                     || contains (s, '<')
                     || contains (s, '>')
                     || contains (s, '~')
                     || contains (s, '\'')
                     || contains (s, '"')
                     || contains (s, '\\');
    if (! commandLine. empty ())
      commandLine += ' ';
    if (bad)
      commandLine += shellQuote (s);
    else
      commandLine += s;
  }
  return commandLine;
}



ostream* logPtr = nullptr;

bool qc_on = false;
ulong seed_global = 1;
bool sigpipe = false;



// COutErr

#ifndef _MSC_VER
bool COutErr::sameFiles (int fd1, 
                         int fd2)
{ 
  struct stat stat1;
  struct stat stat2;
  fstat (fd1, & stat1);
  fstat (fd2, & stat2);
  return stat1. st_ino == stat2. st_ino;
}
#endif

const COutErr couterr;


// thread
size_t threads_max = 1;

inline thread::id get_thread_id ()
  { return this_thread::get_id (); }

thread::id main_thread_id = get_thread_id ();
  
bool isMainThread ()
  { return threads_max == 1 || get_thread_id () == main_thread_id; }



#ifndef _MSC_VER
size_t get_threads_max_max () 
{
#if __APPLE__
  // stderr << "Compiled for MacOS" << "\n";
  int count;
  size_t count_len = sizeof(count);
  sysctlbyname ("hw.logicalcpu", &count, &count_len, NULL, 0);
  // fprintf(stderr,"you have %i cpu cores", count);
  return count;
#else
  LineInput f ("/proc/cpuinfo");
  size_t n = 0;
  while (f. nextLine ())
    if (isLeft (f. line, "processor"))
      n++;
  return n;
#endif
}
#endif




#ifndef _MSC_VER
namespace 
{

void segmFaultHandler (int /*sig_num*/)
{
  signal (SIGSEGV, SIG_DFL); 
  errorExit ("Segmentation fault", true); 
}


[[noreturn]] void sigpipe_handler (int /*sig_num*/) 
{
  signal (SIGPIPE, SIG_DFL);
  if (sigpipe)
    exit (0);
  exit (1);
}

}
#endif



bool initCommon ()
{
  MODULE_INIT

#ifndef _MSC_VER
  signal (SIGSEGV, segmFaultHandler);  
  signal (SIGPIPE, sigpipe_handler);
#endif
        
#ifdef _MSC_VER
  #pragma warning (disable : 4127)
#endif
  static_assert (numeric_limits<char>::is_signed, "char is signed");
  static_assert (sizeof (long) >= 4);
#ifdef _MSC_VER
  #pragma warning (default : 4127)
#endif

  static_assert (SIZE_MAX == std::numeric_limits<size_t>::max ());

  static_assert (sizeof (size_t) == 8);
    
  return true;
}


namespace
{
  const bool init_ = initCommon ();
}



[[noreturn]] void errorExit (const char* msg,
                             bool segmFault)
// alloc() may not work
{ 
	ostream* os = logPtr ? logPtr : & cerr; 
	
	// time ??
#ifndef _MSC_VER
	const char* hostname = getenv ("HOSTNAME");
//const char* shell    = getenv ("SHELL");
	const char* shell    = getenv ("SHELL");	
	const char* pwd      = getenv ("PWD");
	const char* path     = getenv ("PATH");
#endif
  if (contains (msg, error_caption))  // msg already is the result of errorExit()
    *os << endl << msg << endl;
  else
  {
  	*os << endl;
  	{
  	  const OColor oc (*os, Color::red, true, true);
      *os << error_caption;
    }
    *os << endl
        << msg << endl << endl
      #ifndef _MSC_VER
  	    << "HOSTNAME: " << (hostname ? hostname : "?") << endl
  	    << "SHELL: " << (shell ? shell : "?") << endl
  	    << "PWD: " << (pwd ? pwd : "?") << endl
  	    << "PATH: " << (path ? path : "?") << endl
      #endif
  	    << "Progam name:  " << programName << endl
  	    << "Command line: " << getCommandLine () << endl;
  }
  //system (("env >> " + logFName). c_str ());

  os->flush ();           

  if (segmFault)
    abort ();
  exit (1);
}



[[noreturn]] void errorExitStr (const string &msg)
{ 
  errorExit (msg. c_str ()); 
}



namespace
{

string getStack ()
// Print function names:   // --> exec() ??
{
#ifdef _MSC_VER
  return "Stack trace is not implemented";
#else
  string s;
  constexpr size_t size = 100;  // PAR
  void* buffer [size];
  const int nptrs = backtrace (buffer, size);
  if (nptrs <= 1)  // *this function must be the first one
    errorExit (("backtrace size is " + to_string (nptrs)). c_str ());
  char** strings = backtrace_symbols (buffer, nptrs);
  if (strings /*&& ! which ("addr2line"). empty ()*/) 
  {
    FOR_START (int, i, 1, nptrs)
      s += string (strings [i]) + "\n";  
    s += "Use: addr2line -f -C -e " + programArgs [0] + "  -a <address>";
  //free (strings);
  }
  else
    s = "Cannot get stack trace";

  return s;
#endif
}

}



void throwf (const string &s)  
{ 
  if (cxml)
  {
    const Xml::Tag xml (*cxml, "ERROR");
    *cxml << s;
  }
  throw logic_error (s + "\nStack:\n" + getStack ()); 
}





// Chronometer

bool Chronometer::enabled = false;
  
 

// Chronometer_OnePass

Chronometer_OnePass::Chronometer_OnePass (const string &name_arg,
                                          ostream &os_arg,
                                          bool addNewLine_arg,
                                          bool active_arg)
: name (name_arg)
, os (os_arg)
, addNewLine (addNewLine_arg)
, active (active_arg)
, start (active_arg ? time (nullptr) : 0)
{}



Chronometer_OnePass::~Chronometer_OnePass ()
{ 
  if (! active)
    return;
  if (uncaught_exceptions ())
    return;
    
  const time_t stop = time (nullptr);
  
  {
    const OColor oc (os, Color::magenta, false, true /*& os == & cerr*/);  
    os << "CHRON: " << name << ": ";
    const ONumber on (os, 0, false);
    os << difftime (stop, start) << " sec." << endl;  
  }

  if (addNewLine)
    os << endl;
}




//

namespace
{
	
size_t powInt_ (size_t a,
                size_t b)
// Input: a: !0, != 1
{
	if (! b)
		return 1;
	if (b == 1)
		return a;
	const size_t half = b / 2;
	const size_t res = powInt_ (a, half);
	return res * res * (divisible (b, 2) ? 1 : a);
}
	
}


size_t powInt (size_t a,
               size_t b)
{
	if (a)
		if (a == 1)
			return 1;
		else
			return powInt_ (a, b);
	else
		if (b)
			return 0;
		else
			throw runtime_error ("powInt: 0^0");
}




// string

const string noString;



bool isRight (const string &s,
              const string &right)
{
  if (s. size () < right. size ())
    return false;
  return s. substr (s. size () - right. size ()) == right;
}



bool trimPrefix (string &s,
                 const string &prefix)
{
  if (! isLeft (s, prefix))
    return false;
  s. erase (0, prefix. size ());
  return true;
}



bool trimSuffix (string &s,
                 const string &suffix)
{
  if (! isRight (s, suffix))
    return false;
  s. erase (s. size () - suffix. size ());
  return true;
}



void trimSuffixNonAlphaNum (string &s)
{
  for (;;)
  {
    trimTrailing (s);
    if (s. empty ())
      break;
    const size_t pos = s. size () - 1;
    if (   isAlpha (s [pos])
        || isDigit (s [pos])
       )
      break;
    s. erase (pos);
  }
}



bool trimTailAt (string &s,
                 const string &tailStart)
{
  const size_t pos = s. find (tailStart);
  const bool trimmed = pos != string::npos;
  if (trimmed)
    s. erase (pos);
  return trimmed;
}



string pad (const string &s,
            size_t size,
            ebool right)
{
  if (s. size () >= size)
    return s. substr (0, size);
  
  string sp;
  while (sp. size () + s. size () < size)
    sp += ' ';
    
  switch (right)
  {
    case efalse: return sp + s; 
    case etrue:  return s + sp; 
    case enull:  
      {
        const size_t half = sp. size () / 2;
        return sp. substr (0, half) + s + sp. substr (0, sp. size () - half);
      }
  }
  NEVER_CALL;
}



bool goodName (const string &name)
{
  if (name. empty ())
    return false;
  if (name. front () == ' ')
    return false;
  if (name. back () == ' ')
    return false;

  for (const char c : name)  
    if (! printable (c))
      return false;
      
  return true;
}



bool isIdentifier (const string& name,
                   bool dashInName)
{
  if (name. empty ())
    return false;
  if (isDigit (name [0]))
    return false;
  if (dashInName && name [0] == '-')
    return false;
  for (const char c : name)
    if (! isLetter (c) && (! dashInName || c != '-'))
      return false;
  return true;
}



bool isNatural (const string& name)
{
  if (name. empty ())
    return false;
  for (const char c : name)
    if (! isDigit (c))
      return false;
  if (name. size () == 1)
    return true;
  if (name [0] == '0')
    return false;
  return true;
}



bool strBlank (const string &s)
{
  for (const char c : s)
    if (! isSpace (c))
      return false;
  return true;
}



void strUpper (string &s)
{
  for (char& c : s)
    c = toUpper (c);
}



void strLower (string &s)
{
  for (char& c : s)
    c = toLower (c);
}



bool isUpper (const string &s)
{
  for (const char c : s)
    if (! isUpper (c))
    	return false;
  return true;
}



bool isLower (const string &s)
{
  for (const char c : s)
    if (! isLower (c))
    	return false;
  return true;
}



string::const_iterator stringInSet (const string &s,
                    	           	  const string &charSet)
{
  CONST_ITER (string, it, s)
    if (! charInSet (*it, charSet))
      return it;

  return s. end ();
}



size_t strCountSet (const string &s,
 		                const string &charSet)
{
  size_t n = 0;
  for (const char c : s)
	  if (charInSet (c, charSet))		
	    n++;
  return n;
}



void strDeleteSet (string &s,
		               const string &charSet)
{
  FOR_REV (size_t, i, s. size ())
    if (charInSet (s [i], charSet))
    	s. erase (i, 1);
}



void trimLeading (string &s)
{
  size_t i = 0;
  for (; i < s. size () && isSpace (s [i]); i++)
    ;
  s. erase (0, i);
}



void trimTrailing (string &s)
{
	size_t i = s. size ();
	while (i)
		if (isSpace (s [i - 1]))
			i--;
		else
			break;
  s. erase (i);
}



void trimLeading (string &s,
                  char c)
{
  size_t i = 0;
  for (; i < s. size () && s [i] == c; i++)
    ;
  s. erase (0, i);
}



void trimTrailing (string &s,
                   char c)
{
	size_t i = s. size ();
	while (i)
		if (s [i - 1] == c)
			i--;
		else
			break;
  s. erase (i);
}



size_t containsWord (const string& hay,
                     const string& needle)
{
  size_t pos = string::npos;
  for (;;)
  {
    pos = hay. find (needle, pos == string::npos ? 0 : pos + 1);
    if (pos == string::npos)
      break;
    const size_t pos1 = pos + needle. size ();
    if (   (pos  == 0            || ! isLetter (hay [pos - 1]))
        && (pos1 == hay. size () || ! isLetter (hay [pos1]))
       )
      break;
  }
  return pos;  
}



void replace (string &s,
              char from,
              char to)
{ 
  for (char& c : s)
	  if (c == from)
	  	c = to;
}



void replace (string &s,
              const string &fromChars,
              char to)
{
  for (char& c : s)
	  if (charInSet (c, fromChars))
	  	c = to;
}



void replaceStr (string &s,
                 const string &from,
                 const string &to)
{
  ASSERT (! from. empty ());
  
  if (from == to)
    return;
  
  const bool inside = contains (to, from);

  size_t start = 0;    
  for (;;)
  {
    const size_t pos = s. find (from, start);
    if (pos == string::npos)
      break;
    s. replace (pos, from. size (), to);
    start = pos + (inside ? to. size () : 0);
  }
}
  
  
  
string strQuote (const string &s,
                 char quote)
{ 
	string s1 (s);
	replaceStr (s1, "\\", "\\\\");
	const char slashQuote [] = {'\\', quote, '\0'};
	replaceStr (s1, string (1, quote), slashQuote);
	return quote + s1 + quote; 
}



string to_c (const string &s)
{
  string r;
  for (const char c : s)
    if (c == '\n')
      r += "\\n";
    else
    {
      if (charInSet (c, "\"\'\\"))
        r += '\\';
      r += c;
    }   
  return r;
}



void collapseSpace (string &s)
{
  string s1;
  do 
  {
    s1 = s;
    replaceStr (s, "  ", " ");
  }
  while (s1 != s);
}
               


  
string str2streamWord (const string &s,
                       size_t wordNum)
{
  istringstream iss (s);
  string word;
  FOR (size_t, i, wordNum + 1)
    if (iss. eof ())
    	return noString;
    else
      iss >> word;
  return word;
}
                


string str2sql (const string &s)
{
	string r = "'";
  for (const char c : s)
	{
	  if (c == '\'')
	  	r += "'";
	  r += c;
	}
	r += "'";
	
	return r;
}



string sql2escaped (const string &s)
{
  string s1;
  for (const char c : s)
  {
    if (charInSet (c, "[]*_%\\"))
      s1 += '\\';
    s1 += c;
  }
  
  return s1;
}



string findSplit (string &s,
                  char c)
{
	const size_t pos = s. find (c);
	if (pos == string::npos)
	{
		const string s1 (s);
		s. clear ();
		return s1;
	}
	const string before (s. substr (0, pos));
	s. erase (0, pos + 1);
	return before;
}



string rfindSplit (string &s,
                   char c)
{
	const size_t pos = s. rfind (c);
	if (pos == string::npos)
	{
		const string s1 (s);
		s. clear ();
		return s1;
	}
	const string after (s. substr (pos + 1));
	s. erase (pos);
	return after;
}



void reverse (string &s)
{
  FFOR (size_t, i, s. size () / 2)
    swap (s [i], s [s. size () - 1 - i]);
}



string unpercent (const string &s)
{
  for (const char c : s)
  	if (between (c, '\0', ' ') /*! printable (c)*/)
  		throwf (FUNC "Non-printable character: " + to_string (uchar (c)));

  string r;
  constexpr size_t hex_pos_max = 2;
  size_t hex_pos = no_index;
  uchar hex = 0;
  for (const char c : s)
    if (hex_pos == no_index)
    {
      if (c == '%')
        hex_pos = 0;
      else
        r += c;
    }
    else
    {
      ASSERT (hex_pos < hex_pos_max);
      if (! isHex (c))
        throwf (FUNC "Bad hexadecimal character: " + to_string (c));
      hex = uchar (hex + hex2uchar (c));
      if (! hex_pos)
      {
        ASSERT (hex < 16);
        hex = uchar (hex * 16);
      }
      hex_pos++;
      if (hex_pos == hex_pos_max)
      {
        r += char (hex);
        hex_pos = no_index;
        hex = 0;
      }
    }
      
  return r;
}



List<string> str2list (const string &s,
                       char c) 
{
	List<string> res;
	string s1 (s);
	while (! s1. empty ())
	  res << move (findSplit (s1, c));
	return res;
}



string list2str (const List<string> &strList,
                 const string &sep) 
{
	string s;
	bool first = true;
	for (const string& e : strList)
	{
    if (! first)
			s += sep;
	  s += e;
	  first = false;
	}
	return s;
}



bool fileExists (const string &fName)
{ 
#ifdef _MSC_VER
  const ifstream f (fName. c_str ());
  bool ok = f. good ();
#if 0
  if (ok)
    ok = ! directoryExists (fName);
#endif
  return ok;
#else
  return getFiletype (fName, true) == Filetype::file;
#endif
}



streamsize getFileSize (const string &fName)
{
  ifstream f (fName, ifstream::binary);
  if (! f. good ())
    throw runtime_error ("Cannot open file " + shellQuote (fName) + " to get file size");

  const streampos start = f. tellg ();
  QC_ASSERT (start >= 0); 

  f. seekg (0, ios_base::end);
  if (! f. good ())
    throw runtime_error ("Cannot go to the beginning of the file " + shellQuote (fName));

  const streampos end = f. tellg ();
  QC_ASSERT (end >= 0); 

  if (end < start)
    throw runtime_error ("Bad file " + shellQuote (fName));    
  const streampos len = end - start;
  ASSERT (len >= 0);
  QC_ASSERT (len <= (streampos) numeric_limits<streamsize>::max());
  
  return (streamsize) len; 
}



void copyText (const string &inFName,
               size_t skipLines,
               ostream &os)
{
  QC_ASSERT (os. good ());
  LineInput li (inFName);
  while (li. nextLine ())
    if (li. lineNum > skipLines)
      os << li. line << endl;
}



#ifndef _MSC_VER
string filetype2name (Filetype t)
{
  switch (t)
  {
    case Filetype::none:     return "none";
    case Filetype::dir:      return "dir";
    case Filetype::terminal: return "terminal";
    case Filetype::disk:     return "disk";      
    case Filetype::file:     return "file";
    case Filetype::pipe:     return "pipe";
    case Filetype::link:     return "link";
    case Filetype::socket:   return "socket";
    default:
      NEVER_CALL;
  }
}



Filetype getFiletype (const string &path,
                      bool expandLink)
{
  struct stat buf;
  if (expandLink)
  {
    if (stat (path. c_str (), & buf))
      return Filetype::none;
  }
  else
  {
    if (lstat (path. c_str (), & buf))
      return Filetype::none;
  }
    
  if (S_ISDIR  (buf. st_mode))  return Filetype::dir;
  if (S_ISCHR  (buf. st_mode))  return Filetype::terminal;
  if (S_ISBLK  (buf. st_mode))  return Filetype::disk;
  if (S_ISREG  (buf. st_mode))  return Filetype::file;
  if (S_ISFIFO (buf. st_mode))  return Filetype::pipe;
  if (! expandLink)
  {
    if (S_ISLNK  (buf. st_mode))  return Filetype::link;
  }
  if (S_ISSOCK (buf. st_mode))  return Filetype::socket;
    
  NEVER_CALL;
}



void createDirectory (const string &dirName)
{
  if (mkdir (dirName. c_str (), 0777) != 0)  // PAR
    throw runtime_error ("Cannot create directory " + strQuote (dirName));
}



void removeDirectory (const string &dirName)
{
  DirItemGenerator dig (0, dirName, false);
  string item;
  while (dig. next (item))
  {
    const string name (dirName + "/" + item);
    const Filetype t = getFiletype (name, false);
    switch (t)
    {
      case Filetype::link: 
        if (unlink (name. c_str ()))
          throw runtime_error ("cannot unlink " + strQuote (name));
        break;
      case Filetype::dir:
        removeDirectory (name);
        break;
      case Filetype::file:
        removeFile (name);
        break;
      default:
        throw runtime_error ("Cannot remove directory item " + strQuote (name) + " of type " + strQuote (filetype2name (t)));
    }
  }
  if (rmdir (dirName. c_str ()))
    throw runtime_error ("Cannot remove directory " + strQuote (dirName));
}



void concatTextDir (const string &inDirName,
                   const string &outFName)
{
  DirItemGenerator dig (0, inDirName, false);
  OFStream outF (outFName);
  string item;
  while (dig. next (item))
    copyText (inDirName + "/" + item, 0, outF);
}
#endif




// Dir

Dir::Dir (const string &dirName)
{
  ASSERT (! dirName. empty ());
  
  items = str2list (dirName, fileSlash);

  auto it = items. begin (); 
  while (it != items. end ())
    if (it->empty () && it != items. begin ())
    {
      auto it1 = items. erase (it);
      it = it1;
    }
    else 
      it++;
      
  it = items. begin (); 
  while (it != items. end ())
    if (*it == ".")
    {
      auto it1 = items. erase (it);
      it = it1;
    }
    else 
      it++;
      
  it = items. begin (); 
  while (it != items. end ())
    if (*it == ".." && it != items. begin ())
    {
      auto it1 = items. erase (it);
      it1--;
      if (it1->empty ())
      {
        ASSERT (it1 == items. begin ());
        *it1 = "..";
        it = it1;
      }
      else
      {
        auto it2 = items. erase (it1);
        it = it2;
      }
    }
    else
      it++;
}



#ifndef _MSC_VER
size_t Dir::create ()
{
  if (items. empty ())
    throw runtime_error ("Cannot create the root directory");

  const string path (get ());

  if (getFiletype (path, true) == Filetype::dir)
    return 0;

  const string item (items. popBack ());
  const size_t n = create ();
  items << item;
  if (mkdir (path. c_str (), 0777) != 0)  // PAR
    throw runtime_error ("Cannot create directory " + strQuote (path));

  return n + 1;
}
#endif




//

void setSymlink (string path,
                 const string &fName,
                 bool pathIsAbsolute)
{ 
  ASSERT (! path. empty ());
  ASSERT (! fName. empty ());
  const string err (string ("Cannot make ") + (pathIsAbsolute ? "an absolute" : "a relative") + " symlink for " + strQuote (path) + " as " + strQuote (fName));
  if (pathIsAbsolute)
    path = path2canonical (path);
  else
  { 
    if (path [0] == '/')
      throw runtime_error (err + " because " + strQuote (path) + " is absolute");
    const Dir dir (fName);
    const string absPath (dir. getParent () + "/" + path);
    if (getFiletype (absPath, false) == Filetype::none)
      throw runtime_error (err + " because " + strQuote (absPath) + " does not exist");
  }
  if (symlink (path. c_str (), fName. c_str ()))
    throw runtime_error (err);
}



size_t strMonth2num (const string& month)
{
  if (isDigit (month [0]))
  {
    const int m = str2<int> (month);
    ASSERT (m >= 1);
    ASSERT (m <= 12);
    return (size_t) (m - 1);
  }
  
	size_t i = no_index;
	     if (month == "Jan")  i = 0;
  else if (month == "Feb")  i = 1;
  else if (month == "Mar")  i = 2;
  else if (month == "Apr")  i = 3;
  else if (month == "May")  i = 4;
  else if (month == "Jun")  i = 5;
  else if (month == "Jul")  i = 6;
  else if (month == "Aug")  i = 7;
  else if (month == "Sep")  i = 8;
  else if (month == "Oct")  i = 9;
  else if (month == "Nov")  i = 10;
  else if (month == "Dec")  i = 11;
  else 
  	ERROR;
  	
  return i;
}




// istream

bool getChar (istream &is,
              char &c)
{
  QC_ASSERT (is. good ());

  const int i = is. get ();
  c = EOF;
  if (i == c)
  {
    QC_ASSERT (is. eof ());
    return false;
  }
  if (! (i >= 0 && i <= 255))
    throw runtime_error ("Cannot read character: " + to_string (i));
  c = static_cast<char> (i);

  return true;
}



void skipLine (istream &is)
{
  char c;
  while (getChar (is, c) && c != '\n')  // UNIX
    ;
}



void readLine (istream &is,
               string &s)
{
  s. clear ();
  for (;;)
  {	
    char c;
    if (! getChar (is, c))
      break;
	  if (c == '\n')  // UNIX
	  	break;
	  s += c;
	}
}




hash<string> str_hash;
hash<size_t> size_hash;




// Rand

const long Rand::max_ = 2147483647;



ulong Rand::get (ulong max)
{
  ASSERT (max > 0);
  ASSERT (max <= (ulong) max_);
  
  const long up = max_ - (max_ % (long) max);  // Multiple of max
  do run ();
    while (seed >= up);  // P(stay in cycle) < 0.5
  return (ulong) (seed % (long) max);
}



void Rand::qc () const
{
  if (! qc_on)
    return;
  QC_ASSERT (seed > 0);
  QC_ASSERT (seed < max_);
}



void Rand::run ()
{
	qc ();
  
  const long ia = 16807;
  const long iq = 127773;
  const long ir = 2836;

  const long k = seed / iq;
  seed = ia * (seed - k * iq) - ir * k;
  if (seed < 0)
  	seed += max_;
}




// Threads

size_t Threads::threadsToStart = 0;
bool Threads::quiet = false;
	
	


// verbose

namespace
{
	int verbose_ = 0;
}


int getVerbosity ()
{
  return verbose_;
}


bool verbose (int inc)
{ 
  if (! Verbose::enabled ())
    return false;
	return verbose_ + inc > 0;
}


Verbose::Verbose (int verbose_arg)
: verbose_old (verbose_)
{
	if (enabled ())
	  verbose_ = verbose_arg;
}


Verbose::Verbose ()
: verbose_old (verbose_)
{
	if (enabled ())
		verbose_++;
}


Verbose::~Verbose ()
{
	if (enabled ())
		verbose_ = verbose_old;
}


Unverbose::Unverbose ()
{
	if (Verbose::enabled ())
	  verbose_--;
}


Unverbose::~Unverbose ()
{
	if (Verbose::enabled ())
	  verbose_++;
}




//

void exec (const string &cmd,
           const string &logFName)
{
  ASSERT (! cmd. empty ());
  
//Chronometer_OnePass cop (cmd);  
  if (verbose ())
  	cout << cmd << endl;
  if (logPtr)
  	*logPtr << cmd << endl;
  	
	const int status = system (cmd. c_str ());  // pipefail's are not caught
	if (logPtr)
	  *logPtr << "status = " << status << endl;	
	if (status)
	{
	  string err (cmd + "\nstatus = " + to_string (status));
	  if (! logFName. empty ())
	  {
	    const StringVector vec (logFName, (size_t) 10, false);  // PAR
	    err += "\n" + vec. toString ("\n");
	  }
		throw runtime_error (err);		
	}
}



#ifndef _MSC_VER
string which (const string &progName)
{
  ASSERT (! progName. empty ());

  const List<string> paths (str2list (getenv ("PATH"), ':'));
  for (const string& path : paths)
    if (   ! path. empty () 
        && fileExists (path + "/" + progName)
       )
      return path + "/";

  return noString;
}
#endif




// Threads

Threads::Threads (size_t threadsToStart_arg, 
                  bool quiet_arg)
{ 
  if (! isMainThread ())
	  throw logic_error ("Threads are started not from the main thread");
  if (! empty ())
	  throw logic_error ("Previous threads have not finished");

	threadsToStart = threadsToStart_arg;
	if (threadsToStart >= threads_max)
		throw logic_error ("Too many threads to start");
		
  quiet = quiet_arg;
		
	threads. reserve (threadsToStart);
	
	if (! quiet && verbose (1) && threadsToStart)
	{
    const OColor c (cerr, Color::green, false, true);  // Cf. Progress::report() 
    cerr << "# Threads started: " << threadsToStart + 1/*main thread*/ << endl;
  }
}	



Threads::~Threads ()
{ 
  {
    Progress prog (threads. size () + 1, ! quiet);
    const string step ("consecutive threads finished");
    prog (step);  // Main thread
    for (auto& t : threads) 
    { 
      t. join ();
      prog (step);
  	}
  }
	  
	threads. clear ();
	threadsToStart = 0;
	quiet = false;
}





// Xml::Tag

Xml::Tag::Tag (Xml::File &f_arg,
               const string &name_arg)
: name (name_arg)
, f (f_arg)
{ 
  if (contains (name, ' '))
    throwf ("tag name contains a space: " + strQuote (name_arg));
  if (f. printOffset)
  {
    f. print ("\n");
    f. offset++;
    FOR (size_t, i, f. offset * File::offset_spaces)
      f. print (" ");
  }
  if (! name. empty ())
    f. print ("<" + name + ">");
}



Xml::Tag::~Tag ()
{ 
  if (f. printOffset)
  {
    if (f. printBrief)
    {
      f. offset--;
      return;
    }
    else
    {
      f. print ("\n");
      FOR (size_t, i, f. offset * File::offset_spaces)
        f . print (" ");
      f. offset--;
    }    
  }
  if (! name. empty ())
    f. print ("</" + name + ">");
  if (! f. printOffset)
    f. print ("\n");
}




unique_ptr<Xml::File> cxml;
  


// Root

void Root::saveFile (const string &fName) const
{ 
  if (fName. empty ())
		return;
  OFStream f (fName);  // Declared after Root
  saveText (f);
}




void Root::trace (ostream& os,
                  const string& title) const
{ 
  if (! verbose ())
    return;
  Offset::newLn (os);
  os << title << ": ";
  saveText (os);
}




// Named

void Named::qc () const
{
  if (! qc_on)
    return;
  Root::qc ();
    
  if (! goodName (name))
    throw runtime_error ("Bad name: " + strQuote (name));
}




// StringVector

StringVector::StringVector (const string &fName,
                            size_t reserve_size,
                            bool trimP)
{
	searchSorted = true;
	
	reserve (reserve_size);
  try
  {
  	LineInput f (fName);
  	string prev;
    while (f. nextLine ())
    {
      if (trimP)
        trim (f. line);
      *this << f. line;
  	  if (f. line < prev)
  	  	searchSorted = false;
      prev = move (f. line);
    }
  }
  catch (const exception &e)
  {
    throw runtime_error ("Reading file " + shellQuote (fName) + "\n" + e. what ());
  }  
}



StringVector::StringVector (const string &s,
                            char sep,
                            bool trimP)
{
	string s1 (s);
	while (! s1. empty ())
	  *this << move (findSplit (s1, sep));
	if (! s. empty () && s. back () == sep)
	  *this << noString;
	  
	if (trimP)
	  for (string& member : *this)
	    trim (member);
}




string StringVector::toString (const string& sep) const
{ 
  ASSERT (! sep. empty ());
  
  string res;
  for (const string& s : *this)
  { 
    if (! res. empty ())
      res += sep;
    res += s;
  }
  return res;
}



bool StringVector::same (const StringVector &vec,
                         const Vector<size_t> &indexes) const
{
  ASSERT (size () == vec. size ());

  for (const size_t i : indexes)
    if ((*this) [i] != vec [i])
      return false;
  return true;
}




///////////////////////////////////////////////////////////////////////////////////////


// DisjointCluster

void DisjointCluster::merge (DisjointCluster &other)
// Trying to keep rank's small
{ 
  if (this == & other)
    return;
  DisjointCluster* root1 =        getDisjointCluster ();
  DisjointCluster* root2 = other. getDisjointCluster ();
	if (root1->rankDC > root2->rankDC)
		root2->parentDC = root1;
	else
	{
		root1->parentDC = root2;
		if (root1->rankDC == root2->rankDC)
			root2->rankDC ++;
	}
}



DisjointCluster* DisjointCluster::getDisjointCluster ()
{ 
  ASSERT (parentDC);
	if (this == parentDC)
	  return this;
	parentDC = parentDC->getDisjointCluster ();
	return parentDC;
}




// Progress

size_t Progress::beingUsed = 0;



void Progress::report () const
{ 
  if (! n)
    return;
  cerr << '\r';
#ifndef _MSC_VER
  cerr << "\033[2K";
  const OColor c (cerr, Color::green, false, true);  // PAR
#endif
  cerr << n; 
	if (n_max)
		cerr << " / " << n_max;
	if (! step. empty ())
		cerr << ' ' << step;
	cerr << ' ';
	if (! Threads::empty () && ! contains (step, "thread"))
	  cerr << "(main thread) ";
}




// Input

Input::Input (const string &fName,
	            uint displayPeriod)
: ifs (fName)
, is (& ifs)
, prog (0, displayPeriod)  
{ 
  if (! ifs. good ())
    throw runtime_error ("Cannot open file " + shellQuote (fName));
}
 


Input::Input (istream &is_arg,
	            uint displayPeriod)
: is (& is_arg)
, prog (0, displayPeriod)  
{ 
  QC_ASSERT (is);
  if (! is->good ())
    throw runtime_error ("Bad input stream");
}
 


void Input::reset ()
{ 
  ASSERT (is);
  ASSERT (is->good ());

  is->seekg (0); 
  QC_ASSERT (is->good ());
  
  prog. reset ();
}




// LineInput

bool LineInput::nextLine ()
{ 
  ASSERT (is);

	if (eof)  
	{ 
		line. clear ();
	  return false;
	}
	
  try 
	{
    getline (*is, line);  // faster than readLine(*is,line)
  	eof = is->eof ();
  	const bool end = line. empty () && eof;

    if (! commentStart. empty ())
    {
      const size_t pos = line. find (commentStart);
      if (pos != string::npos)
        line. erase (pos);
    }
  //trimTrailing (line); 

  	lineNum++;
  
  	if (! end)
  		prog ();
  		
  	return ! end;
  }
  catch (const exception &e)
  {
    throw runtime_error ("Reading line " + to_string (lineNum + 1) + ":\n" + line + "\n" + e. what ());
  }
}




// ObjectInput

bool ObjectInput::next (Root &row)
{ 
  QC_ASSERT (is);

	row. clear ();

	if (eof)
	  return false;

	row. read (*is);
	row. qc ();
	lineNum++;

 	eof = is->eof ();
  if (eof)
  {
  	ASSERT (row. empty ());
  	return false;
  }

	prog ();
	
  ASSERT (is->peek () == '\n');

  skipLine (*is);

	return true;
}



// CharInput

char CharInput::get ()
{ 
  ASSERT (is);
  
  const char c = (char) is->get ();
	eof = is->eof ();
	
	ASSERT (eof == (c == (char) EOF));

  if (ungot)
  {
    ungot = false;
	  charNum++;
  }
  else
  	if (eol)
  	{ 
  	  lineNum++;
  		charNum = 0;
  		prog ();
    }
    else
  	  charNum++;

	eol = (eof || c == '\n');
	
#if 0
	PRINT (c);
	PRINT (lineNum);
	PRINT (charNum);
#endif
	
	return /*eof ? (char) EOF :*/ c;
}



void CharInput::unget ()
{ 
  ASSERT (is);
	QC_ASSERT (! ungot);
	
  is->unget (); 
	charNum--;  // May be (uint) (-1)
	ungot = true;
}



string CharInput::getLine ()
{
  string s;
  while (! eof)
  {
    const char c = get ();
    if (eol)
      break;
    s += c;
  }
  return s;
}




// Token

void Token::readInput (CharInput &in,
                       bool dashInName_arg,
                       bool consecutiveQuotesInText)
{
	ASSERT (empty ());


  // Skip spaces
	char c = '\0';
	do { c = in. get (); }
	  while (! in. eof && isSpace (c));
	if (in. eof)
		return;  
		
	charNum = in. charNum;

	if (   c == '\'' 
	    || c == '\"'
	   )
	{
		type = eText;
		quote = c;
		bool prevQuote = false;  // Valid if consecutiveQuotesInText
		for (;;)
		{ 
			c = in. get (); 
			if (in. eof && (! consecutiveQuotesInText || ! prevQuote))
		    in. error ("Text is not finished: end of file", false);
			if (in. eol)
		    continue;
			if (c == quote)
			{
			  if (consecutiveQuotesInText)
  			{
  		    if (prevQuote)
  		      prevQuote = false;
  		    else
  		    {
  		      prevQuote = true;
  		      continue;
  		    }
  		  }
  		  else 
  		    break;
  		}
		  else
		    if (consecutiveQuotesInText && prevQuote)
		    {
		      in. unget ();
		      break;
		    }
			name += c;
		}
	}
	else if (isDigit (c) || c == '-')
	{
		while (   ! in. eof 
		       && (   isDigit (c)
		           || c == '.'
		           || c == 'e'
		           || c == 'E'
		           || c == '-'
		           || c == 'x'
		           || isHex (c)
		          )
		      )
		{ 
			name += c;
			c = in. get (); 
		}
		if (! in. eof)
			in. unget ();
	  if (name == "-")
	    type = eDelimiter;
	  else 
	  {
	    type = eText;
	    toNumberDate ();
	  /*QC_ASSERT (   type == eInteger
	               || type == eDouble
	              );*/
	  }
	}
	else if (isLetter (c))
	{
		type = eName;
		dashInName = dashInName_arg;
		while (! in. eof && (isLetter (c) || (dashInName && c == '-')))
		{ 
			name += c;
			c = in. get (); 
		}
		if (! in. eof)
			in. unget ();
	}
	else if (Common_sp::isDelimiter (c))
	{
	  type = eDelimiter;
		name = c;
	}	
	else
	  in. error ("Unknown token starting with ASCII " + to_string ((int) c), false);
	  	    
	ASSERT (! empty ());
  qc ();

  if (verbose ())
  	cout << type2str (type) << ' ' << *this << ' ' << charNum + 1 << endl;
}



void Token::qc () const
{
  if (! qc_on)
    return;
  if (! empty ())
  {
  	QC_IMPLY (type != eText, ! name. empty ());
  	QC_IMPLY (type != eText,  ! contains (name, ' '));
    QC_IMPLY (type == eName || type == eDelimiter, quote == '\0');
    QC_IMPLY (dashInName, type == eName);
  //QC_ASSERT (! contains (name, quote));
    QC_IMPLY (type != eDouble, decimals == 0);
  	switch (type)
  	{ 
  		case eName:      if (! isIdentifier (name, dashInName))
  		                   throw runtime_error ("Not an identifier: " + strQuote (name));
  		                 break;
  	  case eText:      break;
  		case eInteger:   
  		case eDouble:    QC_ASSERT (name [0] == '-' || isDigit (name [0])); 
  		                 break;
  		case eDelimiter: QC_ASSERT (name. size () == 1); 
  		                 QC_ASSERT (Common_sp::isDelimiter (name [0]));
  		                 break;
  		case eDateTime:  break;
  		default: throw runtime_error ("Token: Unknown type");
  	}
  }
}



void Token::saveText (ostream &os) const 
{ 
  if (empty ())
    return;
    
  switch (type)
	{ 
	  case eName:      
	  case eDateTime:  os << name; 
	                   break;
		case eText:      if (quote)
		                   os << quote;
		                 os << name;
		                 if (quote)
		                   os << quote; 
		                 break;
		case eInteger:   os << n;             
		                 break;
		case eDouble:    { 
		                   const ONumber on (os, decimals, scientific); 
		                   os << d; 
		                 } 
		                 break;
		case eDelimiter: os << name;          
		                 break;
 		default: throw runtime_error ("Token: Unknown type");
	}
}



bool Token::operator< (const Token &other) const
{
  LESS_PART (*this, other, type);
  switch (type)
  { 
    case eName:
    case eText:
    case eDateTime:
    case eDelimiter: LESS_PART (*this, other, name); break;
    case eInteger:   LESS_PART (*this, other, n);    break; 
    case eDouble:    LESS_PART (*this, other, d);    break;
  }  
  return false;
}



void Token::toNumberDate ()
{
  if (empty ())
    return;
	if (   type != eName
	    && type != eText
	   )
	  return;	 
	if (contains (name, ' '))
	  return;
	
	{  
  	static const regex date_time_re {R"(\d\d\d\d-\d\d-\d\dT\d\d:\d\d:\d\d\.\d\d\d)"};   // Example: 2018-08-13T16:12:54.487
  	smatch sm;
  	if (regex_match (name, sm, date_time_re))
  	{
  	  type = eDateTime;
  	  return;
  	}
  }	
	  
  try
  {
    if (isLeft (name, "0x"))
	  {
 		  n = (long long) std::stoull (name. substr (2), nullptr, 16); 
   		type = eInteger;
    }
    else if (   contains (name, '.') 
    	       || contains (name, 'e') 
    	       || contains (name, 'E') 
    	      )
	  {
		  d = str2<double> (name);
  		type = eDouble;
      decimals = 0;
      size_t pos = name. find ('.');
      if (pos != string::npos)
      {
        pos++;
        while (   pos < name. size () 
               && isDigit (name [pos])
              )
        {
          pos++;
          decimals++;
        }
      }
      scientific = contains (name, 'e');
	  }
	  else
	  {
	    n = str2<long long> (name);
  		type = eInteger;
		}
  }
  catch (...) {}
}




// TokenInput

Token TokenInput::get ()
{ 
  const Token last_ (last);
  last = Token ();
  if (! last_. empty ())
    return last_;
    
  for (;;)  
  { 
    Token t (ci, dashInName, consecutiveQuotesInText);
    if (t. empty ())
      break;
    if (! t. isDelimiter (commentStart))
      return t;
    ci. getLine ();
  }
  
  return Token ();
}



Token TokenInput::getXmlText ()
{ 
  QC_ASSERT (last. empty ());

  Token t;
  t. charNum = ci. charNum;
  size_t htmlTags = 0;
  bool prevSlash = false;
	for (;;)
	{ 
	  char c = ci. get (); 
		if (ci. eof)
	    ci. error ("XML text is not finished: end of file\n" + t. name, false);
	  if (c == '<')
	  {
	    const char nextChar = getNextChar ();
	    if (nextChar == '/')
	    {
	      if (htmlTags)
	        htmlTags--;
	      else
	        break;
	    }
	    else if (   nextChar != '?' 
	             && nextChar != '!' 
	            )
	      htmlTags++;
	  }
	  else if (c == '>')
	    if (prevSlash)
	    {
	      QC_ASSERT (htmlTags);
	      htmlTags--;
	    }
	  if (isSpace (c))
	    c = ' ';
		t. name += c;
		prevSlash = (c == '/');		  
	}
	
	trim (t. name);
	if (! t. name. empty ())
    t. type = Token::eText;  
  else
    { ASSERT (t. empty ()); }
	
  return t;
}



Token TokenInput::getXmlComment ()
{ 
  QC_ASSERT (last. empty ());

  // Embedded comments ??

  QC_ASSERT (ci. get () == '-');
  QC_ASSERT (ci. get () == '-');

  Token t;
  t. charNum = ci. charNum;
  array<char,4> lastChars {{'-', '-', '\0', '\0'}};
  size_t i = 2; 
	for (;;)
	{ 
	  char c = ci. get (); 
		if (ci. eof)
	    ci. error ("XML comment is not finished: end of file", false);
	  QC_ASSERT (c);
	  if (isSpace (c))
	    c = ' ';

    ASSERT (i < 4);
    lastChars [i] = c;
    i = (i + 1) % 4;
    if (   lastChars [(i + 1) % 4] == '-'
        && lastChars [(i + 2) % 4] == '-'
        && lastChars [(i + 3) % 4] == '>'
       )
      break;
      
    if (lastChars [i])
		  t. name += lastChars [i];
	}
	
	trim (t. name);
	if (! t. name. empty ())
    t. type = Token::eText;  
  else
    { ASSERT (t. empty ()); }
	
  return t;
}



Token TokenInput::getXmlProcessingInstruction ()
{ 
  QC_ASSERT (last. empty ());
  
  // Embedded processing instructions ??

  Token t;
  t. charNum = ci. charNum;
	for (;;)
	{ 
	  char c = ci. get (); 
		if (ci. eof)
	    ci. error ("XML comment is not finished: end of file", false);
	  QC_ASSERT (c);
	  if (isSpace (c))
	    c = ' ';	    
	  if (c == '?')
	    break;
    t. name += c;
	}
	get ('>');
	
	trim (t. name);
	if (! t. name. empty ())
    t. type = Token::eText;  
  else
    { ASSERT (t. empty ()); }
	
  return t;
}



Token TokenInput::getXmlMarkupDeclaration ()
{ 
  QC_ASSERT (last. empty ());
  
  Token t;
  t. charNum = ci. charNum;
	for (;;)
	{ 
	  char c = ci. get (); 
		if (ci. eof)
	    ci. error ("XML comment is not finished: end of file", false);
	  QC_ASSERT (c);
	  if (isSpace (c))
	    c = ' ';	    
	  if (c == '>')
	    break;
    t. name += c;
	}
	
	trim (t. name);
	if (! t. name. empty ())
    t. type = Token::eText;  
  else
    { ASSERT (t. empty ()); }
	
  return t;
}



char TokenInput::getNextChar () 
{ 
  QC_ASSERT (last. empty ());

	char c = '\0';
	do { c = ci. get (); }
	  while (! ci. eof && isSpace (c));

	if (ci. eof)
		return '\0';  
		
  ci. unget ();
  return c;
}




// OFStream

void OFStream::open (const string &dirName,
	                   const string &fileName,
	                   const string &extension)
{ 
	QC_ASSERT (! fileName. empty ());
	QC_ASSERT (! is_open ());
	
	string pathName;
	if (! dirName. empty () && ! isDirName (dirName))
	  pathName = dirName + "/";
	pathName += fileName;
	if (! extension. empty ())
		pathName += "." + extension;
		
	ofstream::open (pathName);

	if (! good ())
	  throw runtime_error ("Cannot create file " + shellQuote (pathName));
}




// PairFile

bool PairFile::next ()
{ 
  if (! f. nextLine ())
	  return false;
	  
  iss. reset (f. line);
  name2. clear ();
  iss >> name1 >> name2;
  
  if (name2. empty ())
  	throw runtime_error ("No pair: " + strQuote (name1) + " - " + strQuote (name2));
  if (! sameAllowed && name1 == name2)
  	throw runtime_error ("Same name: " + name1);
  	
  if (orderNames && name1 > name2)
  	swap (name1, name2);
  	
  return true;
}




// Csv
 
string Csv::getWord ()
{ 
  QC_ASSERT (goodPos ());
  
  size_t start = pos;
  size_t stop = no_index;
  if (s [pos] == '\"')
  {
    pos++;
    start = pos;
    findChar ('\"');
    ASSERT (s [pos] == '\"');
    stop = pos;
  }
  
  findChar (',');
  if (stop == no_index)
    stop = pos;
  pos++;
  
  return s. substr (start, stop - start);
}


  
  
StringVector csvLine2vec (const string &line)
{
  StringVector words;  words. reserve (256);  // PAR
  Csv csv (line);
  string s;
  while (csv. goodPos ())
  {
    s = csv. getWord ();
    trim (s);
    words << move (s);
  }
  return words;
}




// Json

Json::Json (JsonContainer* parent,
            const string& name)
{ 
  ASSERT (parent);
  
  if (const JsonArray* jArray = parent->asJsonArray ())
  {
    ASSERT (name. empty ());  
    var_cast (jArray) -> data << this;
  }
  else if (const JsonMap* jMap = parent->asJsonMap ())
  {
    ASSERT (! name. empty ());  
    ASSERT (! contains (jMap->data, name));
    var_cast (jMap) -> data [name] = this;
  }
  // throw() in a child constructor will invoke terminate() if an ancestor is JsonArray or JsonMap
}



void Json::parse (CharInput& in,
                  const Token& firstToken,
                  JsonContainer* parent,
                  const string& name)
{
  switch (firstToken. type)
  {
    case Token::eName:
      {
        string tokenName (firstToken. name);
        strLower (tokenName);
        if (   tokenName == "null"
            || tokenName == "nan"
           )
          new JsonNull (parent, name);
        else if (tokenName == "true")
          new JsonBoolean (true, parent, name);
        else if (tokenName == "false")
          new JsonBoolean (false, parent, name);
        else
          new JsonString (firstToken. name, parent, name);
      }
      break;
    case Token::eText: new JsonString (firstToken. name, parent, name); break;
    case Token::eInteger: new JsonInt (firstToken. n, parent, name); break;
    case Token::eDouble: new JsonDouble (firstToken. d, firstToken. decimals, parent, name); break;
    case Token::eDelimiter:
      switch (firstToken. name [0])
      {
        case '{': new JsonMap   (in, parent, name); break;
        case '[': new JsonArray (in, parent, name); break;
        default: in. error ("Bad delimiter", false);
      }
      break;
    default: in. error ("Bad token", false);
  }
}



string Json::getString () const
{ 
  const auto* this_ = this;
  if (! this_ || asJsonNull ())
    throw runtime_error ("undefined");
  if (const JsonString* j = asJsonString ())
    return j->s;
  throw runtime_error ("Not a JsonString");
}



long long Json::getInt () const
{ 
  const auto* this_ = this;
  if (! this_ || asJsonNull ())
    throw runtime_error ("undefined");
  if (const JsonInt* j = asJsonInt ())
    return j->n;
  throw runtime_error ("Not a JsonInt");
}



double Json::getDouble () const
{ 
  const auto* this_ = this;
  if (! this_)
    throw runtime_error ("undefined");
  if (asJsonNull ())
    return numeric_limits<double>::quiet_NaN ();
  if (const JsonDouble* j = asJsonDouble ())
    return j->n;
  throw runtime_error ("Not a JsonDouble");
}



bool Json::getBoolean () const
{ 
  const auto* this_ = this;
  if (! this_ || asJsonNull ())
    throw runtime_error ("undefined");
  if (const JsonBoolean* j = asJsonBoolean ())
    return j->b;
  throw runtime_error ("Not a JsonBoolean");
}



const Json* Json::at (const string& name_arg) const
{ 
  const auto* this_ = this;
  if (! this_)
    throw runtime_error ("undefined");
  if (asJsonNull ())
    return nullptr;
  if (const JsonMap* j = asJsonMap ())
    return findPtr (j->data, name_arg);
  throw runtime_error ("Not a JsonMap");
}



const Json* Json::at (size_t index) const
{ 
  const auto* this_ = this;
  if (! this_)
    throw runtime_error ("undefined");
  if (asJsonNull ())
    return nullptr;
  if (const JsonArray* j = asJsonArray ())
  {
    if (index >= j->data. size ())
      throw runtime_error ("Index out of range");
    else
      return j->data [index];
  }
  throw runtime_error ("Not a JsonArray");
}        



size_t Json::getSize () const
{ 
  const auto* this_ = this;
  if (! this_)
    throw runtime_error ("undefined");
  if (asJsonNull ())
    return 0;
  if (const JsonArray* j = asJsonArray ())
    return j->data. size ();
  throw runtime_error ("Not a JsonArray");
}        



// JsonArray


JsonArray::JsonArray (CharInput& in,
                      JsonContainer* parent,
                      const string& name)
: JsonContainer (parent, name)
{
  bool first = true;
  for (;;)
  {
    Token token (in, false, false);
    if (token. isDelimiter (']'))
      break;
    if (! first)
    {
      if (! token. isDelimiter (','))
        in. error ("\',\'");
      token = Token (in, false, false);
    }
    parse (in, token, this, string());
    first = false;
  }
}



void JsonArray::saveText (ostream& os) const
{ 
  os << "[";
  bool first = true;
  for (const Json* j : data)
  {
    ASSERT (j);
    if (! first)
      os << ",";
    j->saveText (os);
    first = false;
  }
  os << "]";
}



// JsonMap

JsonMap::JsonMap ()
{
  ASSERT (! jRoot);
  jRoot = this;
}



JsonMap::JsonMap (const string &fName)
{
  CharInput in (fName);
  const Token token (in, false, false);
  if (! token. isDelimiter ('{'))
    in. error ("Json file " + shellQuote (fName) + ": text should start with '{'", false);
  parse (in);
}



void JsonMap::parse (CharInput& in)
{
  ASSERT (data. empty ());
  
  bool first = true;
  for (;;)
  {
    Token token (in, false, false);
    if (token. isDelimiter ('}'))
      break;
    if (! first)
    {
      if (! token. isDelimiter (','))
        in. error ("\',\'");
      token = Token (in, false, false);
    }
    if (   token. type != Token::eName
        && token. type != Token::eText
       )
      in. error ("name or text");
    string name (token. name);
    const Token colon (in, false, false);
    if (! colon. isDelimiter (':'))
      in. error ("\':\'");
    token = Token (in, false, false);
    Json::parse (in, token, this, name);
    first = false;
  }
}



JsonMap::~JsonMap ()
{ 
  for (auto& it : data)
    delete it. second;
}



void JsonMap::saveText (ostream& os) const
{ 
  os << "{";
  bool first = true;
  for (const auto& it : data)
  {
    if (! first)
      os << ",";
    os << toStr (it. first) << ":";
    const Json* j = it. second;
    ASSERT (j);
    j->saveText (os);
    first = false;
  }
  os << "}";
}




JsonMap* jRoot = nullptr;




// Offset

size_t Offset::size = 0;





// FileItemGenerator

FileItemGenerator::FileItemGenerator (size_t progress_displayPeriod,
                                      const string& fName_arg,
                                      bool tsv_arg)
: ItemGenerator (0, progress_displayPeriod)
, fName (fName_arg)
, tsv (tsv_arg)
{ 
  f. open (fName);
  QC_ASSERT (f. good ()); 
  
  if (tsv)
  {
    string item;
    next (item);
    ASSERT (prog. n == 1);
    prog. n = 0;
  }
}



bool FileItemGenerator::next (string &item)
{ 
  if (f. eof ())
    return false;
    
	readLine (f, item);
  if (! tsv)
    trim (item);
  if (item. empty () && f. eof ())
    return false;

  prog (item);
    
	return true;
}    




#ifndef _MSC_VER
// DirItemGenerator

struct DirItemGenerator::Imp
{
  DIR* dir {nullptr};
  
  
  explicit Imp (const string &dirName)
    : dir (opendir (dirName. c_str ()))
    {
      if (! dir)
        throw runtime_error ("Cannot open directory " + strQuote (dirName));
    }
 ~Imp ()
    {   
      closedir (dir);
    }
};



DirItemGenerator::DirItemGenerator (size_t progress_displayPeriod,
                                    const string& dirName_arg,
                                    bool large_arg)
: ItemGenerator (0, progress_displayPeriod)
, dirName (dirName_arg)
, imp (new Imp (dirName_arg))
, large (large_arg)
{ 
  trimSuffix (dirName,  "/");
}



DirItemGenerator::~DirItemGenerator ()
{ 
  delete imp; 
}



bool DirItemGenerator::next (string &item)
{ 
  if (! large)
    return next_ (item, true);

  for (;;)
  {
    if (! dig. get ())
    {
      string subDir;
      if (! next_ (subDir, false))
        return false;
      dig. reset (new DirItemGenerator (0, dirName + "/" + subDir, false));
      QC_ASSERT (dig. get ());
    }
    if (dig->next (item))
      return true;
    dig. reset (nullptr);
  }
}    



bool DirItemGenerator::next_ (string &item,
                              bool report)
{ 
  ASSERT (imp);
  for (;;)
  {
    if (const dirent* f = readdir (imp->dir))    
      item = move (string (f->d_name));
    else
      return false;
    if (item == ".")
      continue;
    if (item == "..")
      continue;
    break;
  }

  if (report)
    prog (item);

	return true;
}    




StringVector DirItemGenerator::toVector ()
{
  StringVector vec;
  string s;
  while (next (s))
    vec << move (s);
    
  return vec;
}
#endif




// SoftwareVersion

SoftwareVersion::SoftwareVersion (const string &fName)
{ 
  StringVector vec (fName, (size_t) 1, true);
  if (vec. size () != 1)
    throw runtime_error ("Cannot read software version. One line is expected in the file: " + shellQuote (fName));
  init (move (vec [0]), false);
}



SoftwareVersion::SoftwareVersion (istream &is,
                                  bool minorOnly)
{ 
  string s;
  is >> s;
  init (move (s), minorOnly);
}



void SoftwareVersion::init (string &&s,
                            bool minorOnly)
{ 
  try 
  { 
    major = str2<uint> (findSplit (s, '.'));
    if (minorOnly)
      minor = str2<uint> (s);
    else
    { 
      minor = str2<uint> (findSplit (s, '.'));
      patch = str2<uint> (s);
    }
  } 
  catch (...) 
  { 
    throw runtime_error ("Cannot read software version"); 
  }
}



bool SoftwareVersion::operator< (const SoftwareVersion &other) const
{ 
  LESS_PART (*this, other, major);
  LESS_PART (*this, other, minor);
  LESS_PART (*this, other, patch);
  return false;
}




// DataVersion

DataVersion::DataVersion (const string &fName)
{ 
  StringVector vec (fName, (size_t) 1, true);
  if (vec. size () != 1)
    throw runtime_error ("Cannot read data version. One line is expected in the file: " + shellQuote (fName));
  init (move (vec [0]));
}



DataVersion::DataVersion (istream &is)
{ 
  string s;
  is >> s;
  init (move (s));
}



void DataVersion::init (string &&s)
{ 
  try
  { 
    year  = str2<uint> (findSplit (s, '-'));
    month = str2<uint> (findSplit (s, '-'));
    day   = str2<uint> (findSplit (s, '.'));
    num   = str2<uint> (s);
  } 
  catch (...) 
  { 
    throw runtime_error ("Cannot read data version"); 
  }
}



bool DataVersion::operator< (const DataVersion &other) const
{ 
  LESS_PART (*this, other, year);
  LESS_PART (*this, other, month);
  LESS_PART (*this, other, day);
  LESS_PART (*this, other, num);
  return false;
}




// Application

Application::Arg::Arg (const string &name_arg,
                       const string &description_arg)
: Named (name_arg)
, description (description_arg)
{
  trim (name);
  ASSERT (! name. empty ());
  ASSERT (! contains (name, ' '));  
  ASSERT (! contains (name, '='));
  ASSERT (! isLeft (name, "-"));
  ASSERT (! description. empty ());
  ASSERT (name != Application::helpS);
  ASSERT (name != Application::versionS);
}



void Application::Arg::qc () const
{
  if (! qc_on)
  	return;
  	
  QC_ASSERT (! name. empty ());
  QC_ASSERT (! description. empty ());
}



void Application::Key::qc () const
{
  if (! qc_on)
  	return;
  Arg::qc ();
  
  QC_IMPLY (app. gnu, name. size () > 1);
  QC_IMPLY (acronym, app. gnu);
  QC_ASSERT (isUpper (var));
  QC_ASSERT (! var. empty () == (app. gnu && ! flag));
  QC_IMPLY (! requiredGroup. empty (), defaultValue. empty ());
}



void Application::Key::saveText (ostream &os) const
{
  os << '-';
	if (app. gnu)
	{
		os << "-" << name;
		if (! flag)
			os << ' ' << var;
	}
	else
	{
	  os << name; 
	  if (! flag)
	  {
	    os << ' ';
	    const bool quoted = defaultValue. empty () || contains (defaultValue, ' ');
	    if (quoted)
	      os << '\"';
	    os << defaultValue;
	    if (quoted)
	      os << '\"';
	  }
	}
}



string Application::Key::getShortHelp () const
{
	string acronymS;
	if (acronym)
	{
		acronymS = string ("-") + acronym;
		if (! flag)
			acronymS += " " + var;
		acronymS += ", ";
	}
  return acronymS + str ();
}



void Application::addKey (const string &name, 
                          const string &argDescription,
                          const string &defaultValue,
                          char acronym,
                          const string &var)
{
  ASSERT (! name. empty ());
  ASSERT (! contains (name2arg, name));
  if (acronym && contains (char2arg, acronym))
  	throw logic_error ("Duplicate option " + strQuote (string (1, acronym)));
  keys << Key (*this, name, argDescription, defaultValue, acronym, var);
  name2arg [name] = & keys. back ();
  if (acronym)
  	char2arg [acronym] = & keys. back ();
}



void Application::addFlag (const string &name,
                           const string &argDescription,
                           char acronym)
{
  ASSERT (! name. empty ());
  ASSERT (! contains (name2arg, name));
  if (acronym && contains (char2arg, acronym))
  	throw logic_error ("Duplicate option " + strQuote (string (1, acronym)));
  keys << Key (*this, name, argDescription, acronym);
  name2arg [name] = & keys. back ();
  if (acronym)
  	char2arg [acronym] = & keys. back ();
}



void Application::addPositional (const string &name,
                                 const string &argDescription)
{
  ASSERT (! contains (name2arg, name));
  IMPLY (gnu, isUpper (name));
  positionals << Positional (name, argDescription);
  name2arg [name] = & positionals. back ();
}



Application::Key* Application::getKey (const string &keyName) const
{
  if (! contains (name2arg, keyName))
    errorExitStr ("Unknown key: " + strQuote (keyName) + "\n\n" + getInstruction ());
    
  Key* key = nullptr;
  if (const Arg* arg = findPtr (name2arg, keyName))  	
    key = var_cast (arg->asKey ());
  if (! key)
    errorExitStr (strQuote (keyName) + " is not a key\n\n" + getInstruction ());
    
  return key;
}



void Application::setPositional (List<Positional>::iterator &posIt,
	                               const string &value)
{
  if (posIt == positionals. end ())
  {
  	if (isLeft (value, "-"))
      errorExitStr (strQuote (value) + " is not a valid option\n\n" + getInstruction ());
  	else
      errorExitStr (strQuote (value) + " cannot be a positional parameter\n\n" + getInstruction ());
  }
  (*posIt). value = value;
  posIt++;
}



void Application::setRequiredGroup (const string &keyName,
                                    const string &requiredGroup)
{ 
  ASSERT (! requiredGroup. empty ());
	getKey (keyName) -> requiredGroup = requiredGroup; 
}



void Application::addDefaultArgs ()
{ 
  if (gnu)
	{ 
	  addKey ("threads", "Max. number of threads", "1", '\0', "THREADS");
	  addFlag ("debug", "Integrity checks");
    addKey ("log", "Error log file, appended, opened on application start", "", '\0', "LOG");
  }
	else
	{ 
	  addFlag ("qc", "Integrity checks (quality control)");
    addKey ("verbose", "Level of verbosity", "0");
    addFlag ("noprogress", "Turn off progress printout");
    addFlag ("profile", "Use chronometers to profile");
    addKey ("seed", "Positive integer seed for random number generator", "1");
    addKey ("threads", "Max. number of threads", "1");
    addKey ("json", "Output file in Json format");
    addKey ("log", "Error log file, appended");
  #ifndef _MSC_VER
    addFlag ("sigpipe", "Exit normally on SIGPIPE");
  #endif
  }
}



void Application::qc () const
{
  if (! qc_on)
    return;
  
  QC_ASSERT (! description. empty ());
  QC_ASSERT (! version. empty ());
  QC_IMPLY (! needsArg, positionals. empty ());
  
  for (const Positional& p : positionals)
  	p. qc ();
  	
  map<string,size_t> requiredGroups;
  string requiredGroup_prev;
  for (const Key& key : keys)
  {
  	key. qc ();
  	if (! key. requiredGroup. empty () && key. requiredGroup != requiredGroup_prev)
  	{
  		QC_ASSERT (! contains (requiredGroups, key. requiredGroup));
  		requiredGroups [key. requiredGroup] ++;
  	}
  	requiredGroup_prev = key. requiredGroup;
  }
  for (const auto& it : requiredGroups)
  	QC_ASSERT (it. second > 1);
}
	


string Application::getArg (const string &name) const
{
  if (contains (name2arg, name))  
    return name2arg. at (name) -> value;
  throw logic_error ("Parameter " + strQuote (name) + " is not found");
}



bool Application::getFlag (const string &name) const
{
  const string value (getArg (name));
  const Key* key = name2arg. at (name) -> asKey ();
  if (! key || ! key->flag)
    throw logic_error ("Parameter " + strQuote (name) + " is not a flag");
  return value == "true";
}



string Application::key2shortHelp (const string &name) const
{
  if (! contains (name2arg, name))  
    throw logic_error ("Parameter " + strQuote (name) + " is not found");
  const Arg* arg = name2arg. at (name);
  ASSERT (arg);
  if (const Key* key = arg->asKey ())
    return key->getShortHelp ();
  throw logic_error ("Parameter " + strQuote (name) + " is not a key");
}



string Application::getInstruction () const
{
  string instr (description);

  instr += "\n\nUSAGE:   " + programName;

  for (const Positional& p : positionals)
    instr += " " + p. str ();

  string requiredGroup_prev;
  for (const Key& key : keys)
  {
 		if (key. requiredGroup == requiredGroup_prev)
	  	if (key. requiredGroup. empty ())
		    instr += " ";
	  	else
  		  instr += " | ";
  	else
  		if (requiredGroup_prev. empty ())
  			instr += " (";
  		else 
  		{
 			  instr += ") ";
  			if (! key. requiredGroup. empty ())
  				instr += "(";
  		}
	  if (key. requiredGroup. empty ())
	  	instr += "[";
    instr += key. str ();
	  if (key. requiredGroup. empty ())
	  	instr += "]";
  	requiredGroup_prev = key. requiredGroup;
  }
	if (! requiredGroup_prev. empty ())
		instr += ")";
  
  instr += "\nHELP:    " + programName + " " + ifS (gnu, "-") + "-" + helpS;
  if (gnu)
    instr += " or " + programName + " -" + helpS [0];
  instr += "\nVERSION: " + programName + " " + ifS (gnu, "-") + "-" + versionS;

  return instr;
}



string Application::getHelp () const
{
  string instr (getInstruction ());

  const string par ("\n    ");

  if (! positionals. empty ())
  {
	  instr += "\n\nPOSITIONAL PARAMETERS:";
	  for (const Positional& p : positionals)
	    instr += "\n" + p. str () + par + p. description;
	}

  if (! keys. empty ())
  {
	  instr += "\n\nNAMED PARAMETERS:";
	  for (const Key& key : keys)
	  {
	    instr += "\n" + key. getShortHelp () + par + key. description;
	    if (gnu && ! key. defaultValue. empty ())
	    	instr += par + "Default: " + key. defaultValue;
	  }
	}
	
  return instr;
}



int Application::run (int argc, 
                      const char* argv []) 
{
  ASSERT (programArgs. empty ());
  

  addDefaultArgs ();
    
  
	try
  { 
  	// programArgs
    for (int i = 0; i < argc; i++)  
    {
    	// gnu: -abc --> -a -b -c ??
    	string key (argv [i]);
    	string value;
    	bool valueP = false;
    	if (   i 
    		  && ! key. empty () 
    		  && key [0] == '-'
    		 )
    	{
	    	const size_t eqPos = key. find ('=');
	    	if (eqPos != string::npos)
	    	{
	    		value = key. substr (eqPos + 1);
	    		key = key. substr (0, eqPos);
	    		valueP = true;
	    	}
	    }
      programArgs. push_back (key);
      if (valueP)
	      programArgs. push_back (value);
    }
    ASSERT (! programArgs. empty ());
	  
      
		initEnvironment ();


    // positionals, keys
    Set<Key*> keysRead;
    {
	    bool first = true;
		  List<Positional>::iterator posIt = positionals. begin ();
	    Key* key = nullptr;
	    for (string s : programArgs)
	    {
	      if (first)
	      {
	        programName = rfindSplit (s, fileSlash);
	        ASSERT (! programName. empty ());
	      }
	      else if (key)
      	{
          ASSERT (! key->flag);
          key->value = s;
          key = nullptr;
        }
        else if (! s. empty () && s [0] == '-')
        {

          const string s1 (s. substr (1));
          if (s1. empty ())
            errorExitStr ("Dash with no key\n\n" + getInstruction ());

          string name;
          const char c = s1 [0];  // Valid if name.empty()
          if (gnu)
          	if (c == '-')
          	{
          		name = s1. substr (1);
		          if (name. empty ())
		            errorExitStr ("Dashes with no key\n\n" + getInstruction ());
          	}
          	else
          	{
          		if (s1. size () != 1) 
                errorExitStr ("Single character expected: " + strQuote (s1) + "\n\n" + getInstruction ());
            }
          else
          	name = s1;

          if (name == helpS || (name. empty () && c == helpS [0] && gnu))
          {
            cout << getHelp () << endl;
            return 0;
          }
          if (name == versionS)
          {
            cout << version << endl;
            return 0;
          }
          
          // key
          if (name. empty ())
          {
          	ASSERT (gnu);
	          key = var_cast (findPtr (char2arg, c));
	        }
          else
          	if (const Arg* arg = findPtr (name2arg, name))					    
					    key = var_cast (arg->asKey ());
					    
					if (key)
          {
	          if (keysRead. contains (key))
	            errorExitStr ("Parameter " + strQuote (key->name) + " is used more than once");
	          else
	            keysRead << key;
	            
	          if (key->flag)
	          {
	            key->value = "true";
	            key = nullptr;
	          }
	        }
	        else
	        	setPositional (posIt, s);
        }
        else
        	setPositional (posIt, s);

	      first = false;
	    }
      if (key)
        errorExitStr ("Key with no value: " + key->name + "\n\n" + getInstruction ());


	    if (programArgs. size () == 1 && (! positionals. empty () || needsArg))
	    {
	      cout << getInstruction () << endl;
	      return 1;
	    }
	    

	    if (posIt != positionals. end ())
	      errorExitStr ("Too few positional parameters\n\n" + getInstruction ());
	  }
    
    
    for (Key& key : keys)
      if (! keysRead. contains (& key))
        key. value = key. defaultValue;


    {
	    map<string,StringVector> requiredGroup2names;
	    map<string,StringVector> requiredGroup2given;
	    for (const Key& key : keys)
	    	if (! key. requiredGroup. empty ())
	    	{
	    		if (! key. value. empty ())
	    		{
	    			const StringVector& given = requiredGroup2given [key. requiredGroup];
		    		if (! given. empty ())
		    			throw runtime_error ("Parameter " + strQuote (key. name) + " is conflicting with " + given. toString (", "));
		    		requiredGroup2given [key. requiredGroup] << strQuote (key. name);
		    	}
		    	requiredGroup2names [key. requiredGroup] << strQuote (key. name);
	    	}
	    for (const auto& it : requiredGroup2names)
	    	if (requiredGroup2given [it. first]. empty ())
	    		throw runtime_error ("One of required parameters is missing: " + it. second. toString (", "));
	  }
  
  
    string logFName;
    logFName = getArg ("log");
  	ASSERT (! logPtr);
    if (! logFName. empty ())
  		logPtr = new ofstream (logFName, ios_base::app);

    string jsonFName;
    if (gnu)
    {
	  	if (getFlag ("debug"))
	  		qc_on = true;
    }
    else
    {
	  	if (getFlag ("qc"))
	  		qc_on = true;
	
	  	if (getFlag ("noprogress"))
	  		Progress::disable ();
	  	if (getFlag ("profile"))
	  		Chronometer::enabled = true;
	  			
	  	seed_global = str2<ulong> (getArg ("seed"));
	  	if (! seed_global)
	  		throw runtime_error ("seed cannot be 0");
	  
	  	jsonFName = getArg ("json");
	  	ASSERT (! jRoot);
	  	if (! jsonFName. empty ())
	  	{
	  		new JsonMap ();
	  	  ASSERT (jRoot);
	  	}
	  	
    #ifndef _MSC_VER
	  	sigpipe = getFlag ("sigpipe");
    #endif
	  }
  
	
  	threads_max = str2<size_t> (getArg ("threads"));
  	if (! threads_max)
  		throw runtime_error ("Number of threads cannot be 0");	
  	if (threads_max > 1 && Chronometer::enabled)
  		throw runtime_error ("Cannot profile with threads");


  	const Verbose vrb (gnu ? 0 : str2<int> (getArg ("verbose")));
	  	
  
		qc ();
		createTmp ();
  	body ();

  
  	if (! jsonFName. empty ())
  	{
  	  ASSERT (jRoot);
  		OFStream f (jsonFName);
      jRoot->saveText (f);
      delete jRoot;
      jRoot = nullptr;
    }
	}
	catch (const std::exception &e) 
	{ 
	  errorExit (e. what ());
  }


  return 0;
}



#ifndef _MSC_VER
// ShellApplication

ShellApplication::~ShellApplication ()
{
	if (tmpCreated && ! logPtr)
	  removeDirectory (tmp);
}



void ShellApplication::initEnvironment () 
{
  ASSERT (tmp. empty ());
  ASSERT (! programArgs. empty ());
  
  // tmp
  if (useTmp)
  {
    if (const char* s = getenv ("TMPDIR"))
      tmp = move (string (s));
    else
      tmp = "/tmp";
  }

  // execDir, programName
	execDir = getProgramDirName ();
	if (execDir. empty ())
		execDir = which (programArgs. front ());
  if (! isDirName (execDir))
    throw logic_error ("Cannot identify the directory of the executable");
  {
    string s (programArgs. front ());
    programName = rfindSplit (s, fileSlash);
    execDir = getDirName (path2canonical (execDir + programName));
  }

  string execDir_ (execDir);
  trimSuffix (execDir_, "/");				
  for (Key& key : keys)
    if (! key. flag)
      replaceStr (key. defaultValue, "$BASE", execDir_);
}



void ShellApplication::createTmp () 
{
  ASSERT (! tmpCreated);
  
  if (useTmp)
  {
    const string tmpDir (tmp);
    tmp += "/" + programName + ".XXXXXX";
    if (! mkdtemp (var_cast (tmp. c_str ())))
      throw runtime_error ("Error creating a temporary directory in " + tmpDir);
  	if (tmp. empty ())
  		throw runtime_error ("Cannot create a temporary directory in " + tmpDir);

    {
    	const string testFName (tmp + "/test");
      {
        ofstream f (testFName);
        f << "abc" << endl;
        if (! f. good ())
  		    throw runtime_error (tmpDir + " is full, make space there or use environment variable TMPDIR to change location for temporary files");
      }
      removeFile (testFName);
    }
    
    tmpCreated = true;
  }
}



string ShellApplication::getHelp () const 
{
  string help (Application::getHelp ());
  if (useTmp)
    help += "\n\nTemporary directory used is $TMPDIR or \"/tmp\"";
  return help;
}



void ShellApplication::body () const
{
  if (logPtr && useTmp)
    *logPtr << tmp << endl;
  shellBody ();
}



void ShellApplication::findProg (const string &progName) const
{
	ASSERT (! progName. empty ());
	ASSERT (! contains (progName, '/'));
	ASSERT (isDirName (execDir));
	
	string dir;
	if (! find (prog2dir, progName, dir))
	{
		dir = fileExists (execDir + progName)
		        ? execDir
		        : which (progName);
	  if (dir. empty ())
	    throw runtime_error ("Binary " + shellQuote (progName) + " is not found.\nPlease make sure that " 
	                         + shellQuote (progName) + " is in the same directory as " + shellQuote (Common_sp::programName) + " or is in your $PATH.");;
	  prog2dir [progName] = dir;
	}
	  
	ASSERT (isDirName (dir));
}



string ShellApplication::fullProg (const string &progName) const
{
	string dir;
	if (! find (prog2dir, progName, dir))
	  throw runtime_error ("Program " + shellQuote (progName) + " is not found");
	ASSERT (isDirName (dir));
	return shellQuote (dir + progName) + " ";
}
#endif




string ShellApplication::exec2str (const string &cmd,
                                   const string &tmpName,
                                   const string &logFName) const
{
  ASSERT (! contains (tmpName, ' '));
  const string out (tmp + "/" + tmpName);
  exec (cmd + " > " + out, logFName);
  const StringVector vec (out, (size_t) 1, false);
  if (vec. size () != 1)
    throw runtime_error (cmd + "\nOne line is expected");
  return vec [0];  
}



}


