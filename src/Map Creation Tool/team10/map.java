package team10;

import javax.swing.JApplet;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import java.awt.Color;
import javax.swing.JToolBar;
import java.awt.BorderLayout;
import javax.swing.SwingConstants;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.Component;
import java.awt.Dimension;

import javax.swing.Box;
import javax.swing.border.EmptyBorder;
import java.awt.Frame;

public class map extends JApplet {

	/**
	 * Create the applet.
	 */
	public map() {
		setBackground(Color.DARK_GRAY);
		getContentPane().setBackground(Color.DARK_GRAY);	
		JToolBar toolBar = new JToolBar();
		toolBar.setRollover(true);
		toolBar.setBackground(Color.DARK_GRAY);
		toolBar.setOrientation(SwingConstants.VERTICAL);
		getContentPane().add(toolBar, BorderLayout.WEST);
		
		JButton btnWall = new JButton("");
		btnWall.setSelectedIcon(new ImageIcon(map.class.getResource("/team10/Wall_Selected.png")));
		btnWall.setBackground(Color.WHITE);
		btnWall.setIcon(new ImageIcon(map.class.getResource("/team10/Wall.png")));
		toolBar.add(btnWall);
		
		JButton btnPlayer = new JButton("");
		btnPlayer.setBackground(Color.WHITE);
		btnPlayer.setSelectedIcon(new ImageIcon(map.class.getResource("/team10/Player_Selected.png")));
		btnPlayer.setIcon(new ImageIcon(map.class.getResource("/team10/Player.png")));
		toolBar.add(btnPlayer);
		
		JButton btnGoal = new JButton("");
		btnGoal.setBackground(Color.WHITE);
		btnGoal.setSelectedIcon(new ImageIcon(map.class.getResource("/team10/Goal_Selected.png")));
		btnGoal.setIcon(new ImageIcon(map.class.getResource("/team10/Goal.png")));
		btnGoal.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
			}
		});
		toolBar.add(btnGoal);
		
		JButton btnEmpty = new JButton("");
		btnEmpty.setSelectedIcon(new ImageIcon(map.class.getResource("/team10/Empty_Selected.png")));
		btnEmpty.setIcon(new ImageIcon(map.class.getResource("/team10/Empty.png")));
		btnEmpty.setBackground(Color.WHITE);
		toolBar.add(btnEmpty);
		
		Component verticalStrut = Box.createVerticalStrut(20);
		toolBar.add(verticalStrut);
		
		JButton btnUndo = new JButton("Undo");
		toolBar.add(btnUndo);
		
		JPanel panel = new JPanel();
		getContentPane().add(panel, BorderLayout.SOUTH);
		
		JMenuBar menuBar = new JMenuBar();
		getContentPane().add(menuBar, BorderLayout.NORTH);
		menuBar.setBackground(Color.GRAY);
		
		JMenu mnFile = new JMenu("File");
		menuBar.add(mnFile);
		
		JMenuItem mntmSave = new JMenuItem("Save");
		mnFile.add(mntmSave);
		
		JMenuItem mntmExit = new JMenuItem("Exit");
		mnFile.add(mntmExit);
		for(int i = 0; i < 169; i++){
			JLabel label = new JLabel();
			label.setIcon(new ImageIcon(map.class.getResource("/team10/Empty.png")));
			panel.add(label);
		}

	}

}
