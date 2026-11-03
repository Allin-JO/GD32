# Changelog

## [1.2.0] - 2026-11-03

### Added
- GD32H759IMT6 main-controller firmware based on the GD32H7xx standard peripheral library.
- RT-Thread 4.1.1 Cortex-M7 port for the GD32H759 main controller.
- H759 reference board mapping in `firmware/gd32h759-main/BOARD/board_h759.h`.
- GD32H759I-START user guide under `docs/`.

### Changed
- Replaced the GD32F303 main-controller project with a GD32H759 project.
- Updated architecture, task model, hardware notes, submission copy, and registration platform to GD32H759.
- Migrated PWM to TIMER0, encoders to TIMER1/2/3/4, USARTs to USART0/1/2, and ADC to ADC2.
- Updated build scripts, repository checks, and formatting paths for the H759 project.

### Verified
- GD32H759 main controller: Keil ARMCLANG V6.7, 0 errors / 0 warnings.
- STM32F103 spray controller: Keil ARMCC V5.06, 0 errors / 1 vendor-port warning.

## [1.1.0] - 2026-10-17

### Added
- Initial GD32 main-controller migration and RT-Thread task framework.
- GitHub repository structure, build scripts, repository checks, and code formatting configuration.

## [1.0.0] - 2026-10-17

### Added
- Initial RT-Thread migration for the STM32F103 main controller and STM32F103 spray controller.
- Motion, command, display, weight, and spray-control threads.
- Submission document and website copy.
