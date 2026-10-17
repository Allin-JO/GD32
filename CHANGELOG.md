# Changelog

## [1.1.0] - 2026-10-17

### Added
- GD32F303RCT6 main-controller firmware based on the GD32F30x standard peripheral library.
- RT-Thread 4.1.1 Cortex-M4 port for the GD32 main controller.
- GitHub repository structure, build scripts, repository checks, and code formatting configuration.
- GD32 + STM32 system architecture diagram and updated submission document.

### Changed
- Main controller migrated from STM32F103RCT6 to GD32F303RCT6.
- Main controller clock kept at 72 MHz for compatibility with the existing 10 kHz PWM and PI parameters.
- Peripheral mapping updated: TIM2/3/4/5/8 to TIMER1/2/3/4/7, USART1/2/3 to USART0/1/2, ADC1 to ADC0.
- Project-owned C sources normalized to UTF-8 and formatted with clang-format.

### Verified
- GD32F303 main controller: Keil ARMCC V5.06, 0 errors / 0 warnings.
- STM32F103 spray controller: Keil ARMCC V5.06, 0 errors / 1 vendor-port warning.

## [1.0.0] - 2026-10-17

### Added
- Initial RT-Thread migration for the STM32F103 main controller and STM32F103 spray controller.
- Motion, command, display, weight, and spray-control threads.
- Submission document and website copy.
