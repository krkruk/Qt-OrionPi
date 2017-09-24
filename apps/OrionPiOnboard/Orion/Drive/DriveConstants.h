#ifndef DRIVECONSTANTS_H
#define DRIVECONSTANTS_H

namespace CONST {
    namespace SETTINGS {
        namespace DRIVE {
            static constexpr char GROUP_NAME [] = "DRIVE_SYSTEM";
            namespace KEY {
                static constexpr char DRIVE_GROUP_TAG[] = "DRIVE_GROUP_KEY";
                static constexpr char ANGULAR_VELOCITY [] = "ANGULAR_VELOCITY_KEY";
                static constexpr char CURRENT [] = "MOTOR_CURRENT_KEY";
                static constexpr char HEATSINK_TEMP [] = "HEATSINK_TEMPERATURE_KEY";
                static constexpr char PWM [] = "PWM_KEY";
                static constexpr char ERROR_CODE [] = "ERROR_CODE_KEY";
            }

            namespace DEFAULT {
                static constexpr char DRIVE_GROUP_KEY[] = "DRIVE";
                static constexpr char ANGULAR_VELOCITY_KEY [] = "ROT";
                static constexpr char CURRENT_KEY [] = "CUR";
                static constexpr char HEATSINK_TEMP_KEY [] = "TEMP";
                static constexpr char PWM_KEY [] = "PWM";
                static constexpr char ERROR_CODE_KEY [] = "ERR";
            }

            namespace MODE {
                static constexpr char GROUP_NAME [] = "DRIVE_MODE";

                namespace DIRECT {
                    namespace KEY {
                        static constexpr char LEFT_WHEEL_ROW [] = "DIRECT/LEFT_WHEEL_ROW";
                        static constexpr char RIGHT_WHEEL_ROW [] = "DIRECT/RIGHT_WHEEL_ROW";
                    }
                    namespace DEFAULT {
                        static constexpr char LEFT_WHEEL_ROW_KEY [] = "LROW";
                        static constexpr char RIGHT_WHEEL_ROW_KEY [] = "RROW";
                    }
                }
            }
        }
    }
}
#endif // DRIVECONSTANTS_H
