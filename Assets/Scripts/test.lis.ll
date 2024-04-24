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

define float @TestRet(float %x) {
entry:
  %cmptmp = fcmp ult float %x, 1.000000e+01
  %0 = uitofp i1 %cmptmp to float
  %ifcond = fcmp one float %0, double 0.000000e+00
  br i1 %ifcond, label %then, label %1

then:                                             ; preds = %entry
  %calltmp = call float @TestFunc(float 1.000000e+00, float 3.000000e+00)
  br label %ifcont

1:                                                ; preds = %entry
  %calltmp1 = call float @TestFunc(float 5.000000e+00, float 4.000000e+00)
  br label %ifcont

ifcont:                                           ; preds = %1, %then
  %iftmp = phi float [ <null operand!>, <badref> ], [ @1, <badref> ]
  ret float %iftmp
}

define float @0() {
entry:
  %calltmp = call float @TestRet(float 1.000000e+00)
  ret float %calltmp
}

define float @1() {
entry:
  %calltmp = call float @TestRet(float 1.000000e+02)
  ret float %calltmp
}
