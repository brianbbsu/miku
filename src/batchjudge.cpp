#include<iostream>
#include<algorithm>
#include<cstdio>
#include<sstream>
#include<iomanip>
#include"sandbox.h"

using namespace std;

bool enable_log;

int main(int argc, char *argv[])
{
   if(argc < 8) return -1;
   int problem_id, td, boxid, time_limit, mem_limit;
   string testee(argv[6]), lang(argv[7]);
   stringstream stoi;
   for(int i = 1; i <= 5; ++i)
      stoi << argv[i] << ' ';
   stoi >> problem_id >> td >> boxid >> time_limit >> mem_limit;

   string target;
   if(lang == "python2" || lang == "python3")target = "main.pyc";
   else target = "main.out";

   //init box
   sandboxInit(boxid);
   ostringstream sout;
   sout << "./testdata/" << setfill('0') << setw(4) << problem_id;
   sout << "/input" << setfill('0') << setw(3) << td;
   string tddir(sout.str());
   sout.str("");
   sout << " /tmp/box/" << boxid << "/box/";
   system(("cp " + tddir + sout.str() + "input").c_str());
   system(("chmod 755 " + sout.str()).c_str());
   system(("touch " + sout.str() + "input").c_str());
   system(("touch " + sout.str() + "output").c_str());
   system(("chmod 666 " + sout.str() + "input").c_str());
   system(("chmod 666 " + sout.str() + "output").c_str());
   system(("cp /tmp/box/" + testee + "/box/" + target + sout.str() + target).c_str());

   //set options
   sandboxOptions opt;
   //opt.dirs.push_back("/tmp/box/10/box");
   opt.cgroup = true;
   opt.procs = 1;
   opt.input = "input";
   opt.output = "output";
   opt.errout = "/dev/null";
   sout.str("");
   sout << td;
   opt.meta = "./testzone/META" + sout.str();
   opt.timeout = time_limit;
   opt.mem = mem_limit;
   opt.file_limit = 32;
   opt.fsize_limit = 65536;
   opt.envs.push_back(string("PATH=") + getenv("PATH"));
   if(lang == "python2" || lang == "python3")opt.envs.push_back("HOME=/tmp/box/" + to_string(boxid) + "/box/");

   //invoke box command
   if(lang == "python2")sandboxExec(boxid, opt, "/usr/bin/env python2.7 main.pyc");
   else if(lang == "python3")sandboxExec(boxid, opt, "/usr/bin/env python3.6 main.pyc");
   else sandboxExec(boxid, opt, "main.out");

   return 0;
}

