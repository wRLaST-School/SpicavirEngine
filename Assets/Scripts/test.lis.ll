; ModuleID = 'Testing JIT'
source_filename = "Testing JIT"

define float @TestFunc(float %a, float %b) {
entry:
  %multmp = fmul float %a, %a
  %multmp1 = fmul float 2.000000e+00, %a
  %multmp2 = fmul float %multmp1, %b
  %addtmp = fadd float %multmp, %multmp2
  %multmp3 = fmul float %b, %b
  %addtmp4 = fadd float %addtmp, %multmp3
  ret float %addtmp4
}

define float @0() {
entry:
  %calltmp = call float @TestFunc(float 1.000000e+00, float 3.000000e+00)
  ret float %calltmp
}
