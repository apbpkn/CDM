clear all;
clc
close all

subplot(1,2,1)

stress=load('stress.out');
strain=load('strain.out');

plot(-strain(:,4)*100,(-stress(:,4)+stress(:,3))/2/200,'k-','linewidth',2)


% qq=[0.01 0.85:1:12.88 ];
% QS=interp1(strain(:,1),strain(:,4)*100,qq);
% 
% plot(strain(:,1),strain(:,4)*100,'k-','linewidth',3)

ax=gca;
set(ax, 'FontSize',14)
% xlim([-6 6])
xlabel('\epsilon_a (%)')
ylabel('(\sigma_a-\sigma_r/)2\sigma_c'' (kPa)')
% 
subplot(1,2,2)
% PWP=load('PWP.txt');
% plot([0;PWP(:,1)],[0;PWP(:,2)],'k-','linewidth',3)
% hold on
% 
% 
% load ru.out
% plot([0;ru(:,1)],[0;ru(:,2)],'r-','linewidth',3)

ax=gca;
set(ax, 'FontSize',18)


ru=load('ru.out');
plot(ru(:,1),ru(:,2),'k-','linewidth',3)

xlabel('Shear strain \gamma (%)')
ylabel('Shear stress \tau (kPa)')
ax=gca;
set(ax, 'FontSize',18)
% xlim([-10 10])