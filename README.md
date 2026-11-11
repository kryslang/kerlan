# Kerlan

A hobby x86 (i386) kernel, booted with GRUB via Multiboot.

<!-- TODO: one or two sentences on what makes Kerlan interesting. -->
<!-- TODO: add a screenshot or GIF here: ![Kerlan demo](docs/demo.gif) -->

## Try it in 2 minutes

You don't need to install a cross-compiler. The kernel is prebuilt, so all you need is a way to run it.

### Option 2: QEMU on your machine

1. Install QEMU:

   | OS | Command |
   |----|---------|
   | Debian / Ubuntu | `sudo apt install qemu-system-x86` |
   | Fedora | `sudo dnf install qemu-system-x86` |
   | Arch | `sudo pacman -S qemu-system-x86` |
   | macOS | `brew install qemu` |
   | Windows | `winget install SoftwareFreedomConservancy.QEMU` |

2. Download `kerlan.iso` from the [latest release](https://github.com/kryslang/kerlan/releases/latest).

3. Create a blank 32 MB disk and boot:

   ```sh
   qemu-img create -f raw hdd.img 32M
   qemu-system-i386 \
     -drive format=raw,file=hdd.img,media=disk,index=0 \
     -drive format=raw,file=kerlan.iso,media=cdrom,index=2 \
     -boot d -no-reboot
   ```

   A QEMU window opens and GRUB boots the kernel.

## Building from source

Kerlan needs an `i386-elf` cross-compiler, NASM, GRUB and xorriso. Building the compiler takes a while.

### Natively

Install a cross-compiler at `/usr/local/i386elfgcc` (the path the Makefile expects) and put `i386-elf-ld` on your `PATH`. `Dockerfile.toolchain` is a working recipe, including the exact binutils and GCC versions.

Also install `nasm`, `make`, `grub-mkrescue` (from `grub-pc-bin`), `xorriso` and `mtools`. Then:

```sh
make        # build os-image.iso and hdd.img
make run    # boot in QEMU
make debug  # boot paused, with GDB attached on :1234
make clean
```

## Project layout

| Path | Contents |
|------|----------|
| `kernel/` | Kernel entry point and core code |
| `cpu/` | CPU-specific code (interrupts and so on) |
| `drivers/` | Hardware drivers |
| `libc/` | Minimal C library |
| `link.ld` | Linker script |


## Troubleshooting

- **`i386-elf-gcc: command not found`:** the cross-compiler isn't at `/usr/local/i386elfgcc/bin`.
- **`grub-mkrescue` fails:** install `xorriso` and `mtools`.
- **Debugging:** `make run` writes QEMU's interrupt and CPU-reset log to `qemu.log`. 

## License

* i don't uderstand liceses yet, so there are none for now.
