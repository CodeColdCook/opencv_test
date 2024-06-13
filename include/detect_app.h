#include "detector_node.h"

using namespace std;
using namespace cv;

class DetectApp
{
public:
  DetectApp()
  {
    detect_node_ptr_.reset(new DetectNode());

    // detect_node_ptr_->Run(std::bind(
    //   &DetectApp::ResultCallback,
    //   this,
    //   std::placeholders::_1
    //   ));
      detect_node_ptr_->SetCallback(
        std::bind(
        &DetectApp::ResultCallback, 
        this, 
        std::placeholders::_1
      ));
      detect_node_ptr_->Run();

      // detect_node_ptr_->Run(std::bind(&DetectApp::ResultCallback, this));
  }

  void ResultCallback(std::string &result);
  // void ResultCallback();

private:
  boost::shared_ptr<DetectNode> detect_node_ptr_;
  // DetectNode detect_node_;
  boost::shared_ptr<boost::thread> detect_thread_;
};