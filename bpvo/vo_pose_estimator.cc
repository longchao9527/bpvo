#include <bpvo/vo_pose_estimator.h>
#include <bpvo/vo_frame.h>

namespace bpvo {

VisualOdometryPoseEstimator::VisualOdometryPoseEstimator(const AlgorithmParameters& p)
    : _params(p)
    , _pose_est_params(p)
    , _pose_est_params_low_res(p) {}


std::vector<OptimizerStatistics>
VisualOdometryPoseEstimator::estimatePose(
    const VisualOdometryFrame* ref_frame, const VisualOdometryFrame* cur_frame,
    const Matrix44& T_init, Matrix44& T_est)
{
  std::vector<OptimizerStatistics> ret(ref_frame->numLevels());

  T_est = T_init;
  _pose_estimator.setParameters(_pose_est_params_low_res);
  for(int i = ref_frame->numLevels()-1; i >= _params.maxTestLevel; --i)
  {
    if(i >= _params.maxTestLevel)
      _pose_estimator.setParameters(_pose_est_params);

    ret[i] = _pose_estimator.run(ref_frame->getTemplateDataAtLevel(i),
                                 cur_frame->getDenseDescriptorAtLevel(i),
                                 T_est);
  }

  return ret;
}

}; // bpvo
