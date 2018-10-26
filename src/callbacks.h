
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "macros.h"
#include "core_computation.h"
#include "dev_tools.h"
#include "tools/timer.h"
#include <fstream>
#include <iomanip>
#include <memory>

namespace SpaceH {

    namespace CallBack {

        template<typename ParticleSys, bool Immediate = false>
        class DefaultWriter {
        public:
            SPACEHUB_USING_TYPE_SYSTEM_OF(ParticleSys);

            DefaultWriter(const char *path, Scalar end_time, size_t output_num = 5000)
                    : write_interval_(end_time / output_num) {

                os = std::make_shared<std::ofstream>(path);
                if (os->is_open()) {
                    (*os) << std::scientific << std::setprecision(16);
                } else {
                    SPACEHUB_ERR_MSG(("Fail to open the file" + std::string(path)).c_str());
                }
            }

            inline void operator()(ParticleSys &partc) {
                if (partc.time() >= write_time_) {
                    (*os) << partc;
                    if constexpr (Immediate) {
                        (*os) << std::endl;
                    }
                    write_time_ += write_interval_;
                }
            }

        private:
            std::shared_ptr<std::ofstream> os;
            Scalar write_time_{0};
            Scalar write_interval_{0};
            size_t step_{0};
        };

        template<typename ParticleSys, bool Immediate = false>
        class EnergyWriter {
        public:
            SPACEHUB_USING_TYPE_SYSTEM_OF(ParticleSys);

            EnergyWriter(const char *path, Scalar end_time, size_t output_num = 5000)
                    : write_interval_(end_time / output_num) {

                os = std::make_shared<std::ofstream>(path);
                if (os->is_open()) {
                    (*os) << std::scientific << std::setprecision(16);
                } else {
                    SPACEHUB_ERR_MSG(("Fail to open the file" + std::string(path)).c_str());
                }
            }

            inline void operator()(ParticleSys &partc) {
                if (partc.time() >= write_time_) {
                    (*os) << partc.time() / Unit::YEAR << ' '
                          << SpaceH::getTotalEnergy(partc.mass(), partc.pos(), partc.vel()) << ' '
                          << SpaceH::getEnergyErr(partc.mass(), partc.pos(), partc.vel(), partc.bindE()) << ' '
                          << partc.omega() << ' ' << partc.bindE() << "\r\n";
                    if constexpr (Immediate) {
                        (*os) << std::endl;
                    }
                    write_time_ += write_interval_;
                }
            }

        private:
            std::shared_ptr<std::ofstream> os;
            std::shared_ptr<std::ofstream> os_eng;
            Scalar write_time_{0};
            Scalar write_interval_{0};
            size_t step_{0};
        };

        template<typename ParticleSys, bool Immediate = false>
        class CoMWriter {
        public:
            SPACEHUB_USING_TYPE_SYSTEM_OF(ParticleSys);

            CoMWriter(const char *path, Scalar end_time, size_t output_num = 5000)
                    : write_interval_(end_time / output_num) {

                os = std::make_shared<std::ofstream>(path);
                if (os->is_open()) {
                    (*os) << std::scientific << std::setprecision(16);
                } else {
                    SPACEHUB_ERR_MSG(("Fail to open the file" + std::string(path)).c_str());
                }
            }

            inline void operator()(ParticleSys &partc) {
                if (partc.time() >= write_time_) {
                    (*os) << partc.time() / Unit::YEAR << ' '
                          << partc.posCoM() << ' '
                          << partc.velCoM() << "\r\n";
                    if constexpr (Immediate) {
                        (*os) << std::endl;
                    }
                    write_time_ += write_interval_;
                }
            }

        private:
            std::shared_ptr<std::ofstream> os;
            std::shared_ptr<std::ofstream> os_eng;
            Scalar write_time_{0};
            Scalar write_interval_{0};
            size_t step_{0};
        };

        template<typename ParticleSys>
        class ShowProgressBar {
        public:
            SPACEHUB_USING_TYPE_SYSTEM_OF(ParticleSys);

            explicit ShowProgressBar(Scalar end_time) : bar(end_time) {}

            inline void operator()(ParticleSys &partc) {
                bar.autoShow(partc.time());
            }

        private:
            Progress bar;
        };

    }
}
#endif

