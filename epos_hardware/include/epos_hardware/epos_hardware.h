#ifndef EPOS_HARDWARE_EPOS_HARDWARE_H_
#define EPOS_HARDWARE_EPOS_HARDWARE_H_

#include <list>
#include <string>
#include <vector>

#include <battery_state_interface/battery_state_interface.hpp>
#include <epos_hardware/epos_diagnostic_updater.h>
#include <epos_hardware/epos_manager.h>
#include <hardware_interface/actuator_command_interface.h>
#include <hardware_interface/actuator_state_interface.h>
#include <hardware_interface/controller_info.h>
#include <hardware_interface/robot_hw.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <ros/node_handle.h>
#include <transmission_interface/robot_transmissions.h>
#include <transmission_interface/transmission_interface_loader.h>

#include <boost/scoped_ptr.hpp>

namespace epos_hardware {

class EposHardware : public hardware_interface::RobotHW {
public:
  EposHardware();
  virtual ~EposHardware();

  bool init(ros::NodeHandle &root_nh, ros::NodeHandle &hw_nh,
            const std::vector< std::string > &motor_names);
  virtual void doSwitch(const std::list< hardware_interface::ControllerInfo > &start_list,
                        const std::list< hardware_interface::ControllerInfo > &stop_list);
  void read();
  void write();
  void updateDiagnostics();

private:
  // subfunctions for init()
  void initLowLevelInterfaces();
  void initMotors(ros::NodeHandle &root_nh, ros::NodeHandle &hw_nh,
                  const std::vector< std::string > &motor_names);
  void initTransmissions(ros::NodeHandle &root_nh);
  void initJointLimits();

private:
  // low level interfaces motors actually have
  hardware_interface::ActuatorStateInterface ator_state_iface_;
  hardware_interface::PositionActuatorInterface pos_ator_iface_;
  hardware_interface::VelocityActuatorInterface vel_ator_iface_;
  hardware_interface::EffortActuatorInterface eff_ator_iface_;
  battery_state_interface::BatteryStateInterface bat_state_iface_;
  EposDiagnosticInterface epos_diag_iface_;

  // bridge between actuator and joint interfaces
  transmission_interface::RobotTransmissions robot_trans_;
  boost::scoped_ptr< transmission_interface::TransmissionInterfaceLoader > trans_iface_loader_;

  // limits related to joint interfaces
  joint_limits_interface::PositionJointSaturationInterface pos_jnt_sat_iface_;
  joint_limits_interface::VelocityJointSaturationInterface vel_jnt_sat_iface_;
  joint_limits_interface::EffortJointSaturationInterface eff_jnt_sat_iface_;

  // motor hardware
  EposManager epos_manager_;
};

} // namespace epos_hardware

#endif
