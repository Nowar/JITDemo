; ModuleID = 'data.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private constant [21 x i8] c"Succeed in Jitting!\0A\00"

define void @bar(i8* %p) nounwind {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([21 x i8]* @.str, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @foo() nounwind {
entry:
  %kerker = alloca i8*, align 8
  %tmp = load i8** %kerker, align 8
  call void @bar(i8* %tmp)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) nounwind {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  call void @foo()
  %0 = load i32* %retval
  ret i32 %0
}
