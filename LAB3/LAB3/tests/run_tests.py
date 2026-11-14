from pathlib import Path
import subprocess,shutil
r=Path(__file__).resolve().parents[1];out=r/'tests/build';out.mkdir(exist_ok=True)
gcc=shutil.which('gcc');logs=[]
for tick in [1,10,100]:
 exe=out/f'test_{tick}.exe'
 cmd=[gcc,'-std=c99','-Wall','-Wextra','-Werror',f'-DTIMER_CYCLE_MS={tick}','-I'+str(r/'tests/mock'),'-I'+str(r/'Core/Inc'),*[str(r/'Core/Src'/p) for p in ['input_reading.c','output_display.c','input_processing.c','timer.c']],str(r/'tests/test_basic.c'),'-o',str(exe)]
 subprocess.run(cmd,check=True)
 result=subprocess.run([str(exe)],capture_output=True,text=True)
 logs.append(result.stdout+result.stderr);print(logs[-1],end='')
 (r/'docs/TEST_RESULTS.txt').write_text(''.join(logs),encoding='utf-8')
 if result.returncode:raise SystemExit(result.returncode)
