{
  description = "C++ development environment with Nix flakes";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
        llvm = pkgs.llvmPackages_latest;
        lib = nixpkgs.lib;
      in {
        devShells.default = pkgs.mkShell {
          name = "cpp-dev-shell";

          buildInputs = with pkgs; [
            # pkgs.clang
            glfw
            glslang # or shaderc
            vulkan-tools
            vulkan-headers
            vulkan-loader
            vulkan-validation-layers

            openal
            pkgs.gdb
            pkgs.lldb
            cmake
            pkg-config
            gcc
            #llvm.lldb
            #llvm.libllvm
            #llvm.libcxx
            # llvm.clang
            clang-tools

            # Audio file reading
            libsndfile
          ];

          #          CPATH = builtins.concatStringsSep ":" [
          #      (lib.makeSearchPathOutput "dev" "include" [ llvm.libcxx ])
          #      (lib.makeSearchPath "resource-root/include" [ llvm.clang ])
          #    ];

          shellHook = ''
            echo "Welcome to the C++ development environment!"
            #export CC=clang
            #export CXX=clang++
            export LIBSNDFILE_INCLUDE_DIR=${pkgs.libsndfile.dev}/include
            export LIBSNDFILE_LIB=${pkgs.libsndfile.dev}/lib/libsndfile.so
            export LIBSNDFILE_PKGCONF=${pkgs.libsndfile.dev}/pkgconfig
            export CC=gcc
            export CXX=g++
          '';
        };
      });
}
