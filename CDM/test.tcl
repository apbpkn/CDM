wipe;
wipe all;

set OutCSR [open CSR_No.dat w]

model BasicBuilder -ndm 3 -ndf 4

set Strength 56
set numCycle 13
set pConf -200.0
set gamma  0.600;
set CSR [expr 0.75]
set dT 0.01
set numSteps [expr 1*$numCycle]
	 
node 1	1.0	0.0	0.0
node 2	1.0	1.0	0.0
node 3 	0.0	1.0	0.0	
node 4	0.0	0.0	0.0

node 5	1.0	0.0	1.0
node 6 	1.0	1.0	1.0
node 7 	0.0	1.0	1.0
node 8 	0.0	0.0	1.0
 
fix 1 	0 1 1 1
fix 2 	0 0 1 1
fix 3	1 0 1 1
fix 4 	1 1 1 1

fix 5	0 1 0 1
fix 6 	0 0 0 1
fix 7	1 0 0 1
fix 8 	1 1 0 1

set GG 1.8e4
set KK [expr 4.6667*$GG];
set alpha [expr 1.0/(4*($KK + (4/3)*$GG))]
# nDMaterial PressureIndependMultiYieldCyclic 1 3 2.0 $GG $KK  $Strength   0.1  2.0    1.2    0.7    0.2    0.99      0.44    0.0  0.0  100.0 0.0 40;

nDMaterial PressureIndependMultiYieldCyclic 1 3 2.0 $GG $KK  $Strength   0.1  2.0    5    0.7    0.18    0.99      0.608    0.0  0.0  100.0 0.0 40;


element SSPbrickUP   1     1 2 3 4 5 6 7 8    1  2.2e6   1.0  100.0  100.0 100.0  0.7   $alpha 

set pNode [expr $pConf / 4.0]
pattern Plain 1 {Series -time {0 10000 1e10} -values {0 1 1} -factor 1} {
	load 1  $pNode  0.0    0.0    0.0
	load 2  $pNode  $pNode 0.0    0.0
	load 3  0.0     $pNode 0.0    0.0
	load 4  0.0     0.0    0.0    0.0
	load 5  $pNode  0.0    $pNode 0.0
	load 6  $pNode  $pNode $pNode 0.0
	load 7  0.0     $pNode $pNode 0.0
	load 8  0.0     0.0    $pNode 0.0
}

constraints Penalty 1.0e18 1.0e18
test        NormDispIncr 1.0e-6 20 0
algorithm   KrylovNewton
numberer    RCM
system      BandGeneral
integrator Newmark $gamma  [expr pow($gamma+0.5, 2)/4] 
analysis VariableTransient 
analyze 150 100 1 100  15	

for {set x 1} {$x<9} {incr x} {
   remove sp $x 4
}
updateMaterialStage -material  1   -stage 1
analyze 50 100 1 100  15

wipeAnalysis
loadConst -time 0.0

remove recorders
recorder Node    -file disp.out   -time -precision 16 -nodeRange 1 8 -dof 1 2 3  disp
recorder Node    -file press.out  -time -precision 16 -nodeRange 1 8 -dof 4    vel
recorder Element -file stress.out -time -precision 16  stress
recorder Element -file strain.out -time -precision 16  strain
recorder Element -file ru.out -time -precision 16  ru

timeSeries Trig 3 0 40000 1 
set P_max [expr -$Strength*2*$CSR]
pattern Plain  2 3 {  
	load  5  0.0 0.0  [expr $P_max/4.0] 0   
	load  6  0.0 0.0  [expr $P_max/4.0] 0    
	load  7  0.0 0.0  [expr $P_max/4.0] 0    
	load  8  0.0 0.0  [expr $P_max/4.0] 0   
}

constraints Transformation
test EnergyIncr 1.0e-4 50  0
numberer   RCM
algorithm Newton
system    ProfileSPD
rayleigh 0.00  0.0  0.001 0.0
integrator Newmark $gamma  [expr pow($gamma+0.5, 2)/4] 
analysis VariableTransient 
while {[expr abs([lindex [eleResponse 1 strain] 2])]<0.1 && [getTime] <$numCycle} {
	analyze 1 $dT [expr $dT/100] $dT 20	
}
puts "Cycle is [getTime]" 
puts $OutCSR "[getTime]" 
wipe all;
wipe;

close $OutCSR