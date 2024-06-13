#include "detector_node.h"

using namespace std;
using namespace cv;
void DetectNode::Process()
{
  sleep(1);
}
void DetectNode::ResultProcess()
{
  static int count = 1;
  if (count % 2 > 0)
    results_.push("true");

  count++;
  count = count % 2;
  while (results_.size() > 3)
    results_.pop();
}