{
  cmake,
  doctest,
  ninja,
  self,
  stdenv,
}:
stdenv.mkDerivation {
  pname = "ap3Can";
  version = "0.0.0";
  src = self;
  doCheck = true; # Auto test
  checkPhase = ''
    runHook preCheck
    ctest --exclude-regex "slcanInterfaceTest"
    runHook postCheck
  '';
  nativeBuildInputs = [
    cmake # Modern build tool
    doctest # Testing framework
    ninja # Modern build tool
    # pkg-config # Build tool
  ];
}
