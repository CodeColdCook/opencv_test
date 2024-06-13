#pragma once
#include "for_test.h"

#include <iostream>
#include <functional>
#include <boost/thread.hpp>
#include <queue>

using namespace std;
using namespace cv;

class DetectNode
{
  typedef std::function<void(std::string &r)> CallbackFun;
  // typedef std::function<void ()> CallbackFun;

public:
  DetectNode() = default;

  void Run(CallbackFun callback)
  {
    processing_ = true;
    while (processing_)
    {
      // boost::this_thread::interruption_point();
      try
      {
        // sleep(1);
        Process();
        ResultProcess();
        if (results_.size() == 0)
        {
          std::string res = "false";
          callback(res);
          // callback();
        }
        else
        {
          callback(results_.front());
          // callback();
          results_.pop();
        }
      }
      catch (boost::thread_interrupted)
      {
        processing_ = false;
        cout << "XTYLODNode main ros thread interrupted!" << endl;
        break;
      }
    }
    cout << "XTYLODNode main ros thread interrupted!" << endl;
    processing_ = false;
  }

  void Run()
  {
    // if(callback_ == )
    processing_ = true;
    while (processing_)
    {
      try
      {
        Process();
        ResultProcess();
        cout << "callback address: " << &callback_ << endl;
        if (results_.size() == 0)
        {
          std::string res = "false";
          callback_(res);
          // callback();
        }
        else
        {
          callback_(results_.front());
          // callback();
          results_.pop();
        }
      }
      catch (const std::exception &e)
      {
        cerr << e.what() << endl;
      }
    }
    cout << "XTYLODNode main ros thread interrupted!" << endl;
    processing_ = false;
  }

  void Process();
  void ResultProcess();

  void SetCallback(CallbackFun callback)
  {
    callback_ = callback;
  }

private:
  bool processing_;
  std::queue<std::string> results_;
  boost::shared_ptr<boost::thread> thread_process_;
  boost::shared_ptr<boost::thread> thread_result_process_;
  float last_pub_time_;
  CallbackFun callback_;
};