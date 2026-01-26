{
  description = "Nix C++ dev env";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  outputs =
    { self, nixpkgs }:
    let
      systems =
        f:
        nixpkgs.lib.genAttrs [
          "aarch64-linux"
          "x86_64-linux"
        ] (system: f (import nixpkgs { inherit system; }));
    in
    {
      packages = systems (pkgs: {
        default = pkgs.callPackage ./pkg.nix { inherit self; };
        # default = self.packages.${pkgs.system}.pkg;
      });
      devShells = systems (pkgs: {
        default = pkgs.mkShell {
          packages = with pkgs; [
            can-utils
            clang-tools # Clang CLIs, including LSP
            cmake-language-server # Cmake LSP
            cmake-format
            cppcheck # Static analysis
            doxygen # Documentation generator
            cppcheck # Static analysis
            lldb # Clang debug adapter
            valgrind # Debugging and profiling
          ];
          nativeBuildInputs = self.packages.${pkgs.system}.default.nativeBuildInputs;
          buildInputs = self.packages.${pkgs.system}.default.buildInputs;
          shellHook = ''
            mkdir --verbose build
            cd build
            cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
          '';
        };
      });
    };
}
