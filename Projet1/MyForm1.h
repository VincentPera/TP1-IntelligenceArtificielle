#pragma once

#include <fstream>
#include <time.h>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "Miner.h"
#include "Drunk.h"
#include "MinersWife.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

#include "ModificationAgents.h"

namespace Projet1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Description résumée de MyForm1
	/// </summary>
	public ref class MyForm1 : public System::Windows::Forms::Form
	{
	public:
		MyForm1(void)
		{
			//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
			os.open("output.txt");
#endif

			//seed random number generator
			srand((unsigned)time(NULL));

			//create entities
			Bob = new Miner(ent_Miner_Bob);
			Elsa = new MinersWife(ent_Elsa);
			Selbastien = new Drunk(ent_SeLbastien);

			//register them with the entity manager
			EntityMgr->RegisterEntity(Bob);
			EntityMgr->RegisterEntity(Elsa);
			EntityMgr->RegisterEntity(Selbastien);

			InitializeComponent();
		}

		void MarshalString(String ^ s, std::wstring& os) {
			using namespace Runtime::InteropServices;
			const wchar_t* chars =
				(const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}

		delegate void SetTextDelegate(String^ text);
		void MyForm1::addTextBob(String ^text)
		{
			if (richTextBox3->InvokeRequired)
			{
				SetTextDelegate^ d = gcnew SetTextDelegate(this, &MyForm1::addTextBob);
				this->Invoke(d, gcnew array<String^> { text });
			}
			else
			{
				richTextBox3->Text += text;
				richTextBox3->SelectionStart = richTextBox3->Text->Length;
				richTextBox3->ScrollToCaret();
			}
		}
		void MyForm1::addTextElsa(String ^text)
		{
			if (richTextBox1->InvokeRequired)
			{
				SetTextDelegate^ d = gcnew SetTextDelegate(this, &MyForm1::addTextElsa);
				this->Invoke(d, gcnew array<String^> { text });
			}
			else
			{
				richTextBox1->Text += text;
				richTextBox1->SelectionStart = richTextBox1->Text->Length;
				richTextBox1->ScrollToCaret();
			}
		}

		delegate void changeStateBobDelegate(String^ text);
		void changeStateBob(String^ state) {
			if (this->label6->InvokeRequired)
			{
				changeStateBobDelegate^ d =
					gcnew changeStateBobDelegate(this, &MyForm1::changeStateBob);
				this->Invoke(d, gcnew array<Object^> { state });
			}
			else
			{
				label6->Text = state;
			}
		}
		delegate void changeStateElsaDelegate(String^ text);
		void changeStateElsa(String^ texte) {
			if (this->label6->InvokeRequired)
			{
				changeStateElsaDelegate^ d =
					gcnew changeStateElsaDelegate(this, &MyForm1::changeStateElsa);
				this->Invoke(d, gcnew array<Object^> { texte });
			}
			else
			{
				label5->Text = texte;
			}
		}
		delegate void changeStateSelbastienDelegate(String^ text);
		void changeStateSelbastien(String^ texte) {
			if (this->label6->InvokeRequired)
			{
				changeStateSelbastienDelegate^ d =
					gcnew changeStateSelbastienDelegate(this, &MyForm1::changeStateElsa);
				this->Invoke(d, gcnew array<Object^> { texte });
			}
			else
			{
				label5->Text = texte;
			}
		}

		// Thread for the agent Bob
		void ThrFunc1()
		{
			for (int i = 0; i<15; i++)
			{
				// update the agent
				Bob->Update();
				// update the IU
				location_type l = Bob->Location();
				this->changeLocationBob(l);
				std::string curr_state = Bob->GetFSM()->GetNameOfCurrentState();
				String^ state = gcnew String(curr_state.c_str());
				this->changeStateBob(state);
				// text

				//this->Refresh();

				//dispatch any delayed messages
				Dispatch->DispatchDelayedMessages();

				Thread::Sleep(800);
			}
			//notify that the thread is done
			this->addTextBob(">> Thread finished.\n");
			//tidy up
			delete Bob;
		}

		// Thread for the agent Elsa
		void ThrFunc2()
		{
			for (int i = 0; i < 7; i++)
			{
				// update the agent
				Elsa->Update();
				// update the IU
				std::string curr_state = Elsa->GetFSM()->GetNameOfCurrentState();
				String^ state = gcnew String(curr_state.c_str());
				this->changeStateElsa(state);
				// text
				//this->Refresh();
				//dispatch any delayed messages
				Dispatch->DispatchDelayedMessages();
				Thread::Sleep(800);
			}
			//notify that the thread is done
			this->addTextElsa(">> Thread finished.\n");
			//tidy up
			delete Elsa;
		}

		// Thread for the agent SelBastien
		void ThrFunc3()
		{
			for (int i = 0; i < 7; i++)
			{
				// update the agent
				Selbastien->Update();
				// update the IU
				std::string curr_state = Selbastien->GetFSM()->GetNameOfCurrentState();
				String^ state = gcnew String(curr_state.c_str());
				this->changeStateSelbastien(state);
				// text
				//this->Refresh();
				//dispatch any delayed messages
				Dispatch->DispatchDelayedMessages();
				Thread::Sleep(800);
			}
			//notify that the thread is done
			this->addTextElsa(">> Thread finished.\n");
			//tidy up
			delete Elsa;
		}

		delegate void changeLocationBobDelegate(int location);
		void changeLocationBob(int location)
		{
			// Request the lock, and block until it is obtained.
			if (this->BobHouse->InvokeRequired && this->BobMine->InvokeRequired &&
				this->BobSaloon->InvokeRequired && this->BobBank->InvokeRequired)
			{
				changeLocationBobDelegate^ d =
					gcnew changeLocationBobDelegate(this, &MyForm1::changeLocationBob);
				this->Invoke(d, gcnew int { location });
			}
			else
			{
				BobHouse->Hide();
				BobMine->Hide();
				BobSaloon->Hide();
				BobBank->Hide();
				switch (location)
				{
				case shack: BobHouse->Show();
					break;
				case goldmine: BobMine->Show();
					break;
				case bank: BobBank->Show();
					break;
				case saloon: BobSaloon->Show();
					break;
				default:
					break;
				}
			}
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~MyForm1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	protected: static Miner* Bob;
	protected: static MinersWife* Elsa;
	protected: static Drunk* Selbastien;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;


	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox4;


	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel3;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel4;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel7;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel8;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel5;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel6;
	private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::RichTextBox^  richTextBox2;
private: System::Windows::Forms::RichTextBox^  richTextBox1;
private: System::Windows::Forms::RichTextBox^  richTextBox3;
















	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  applicationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  lancerToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  quitterToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  agentsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  modifierLesÉtatsToolStripMenuItem;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label6;












	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel9;
private: System::Windows::Forms::PictureBox^  BobHouse;

	private: System::Windows::Forms::PictureBox^  ElsaHouse;
	private: System::Windows::Forms::PictureBox^  SelbastienHouse;



	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel10;
private: System::Windows::Forms::PictureBox^  BobBank;

	private: System::Windows::Forms::PictureBox^  ElsaBank;
	private: System::Windows::Forms::PictureBox^  SelbastienBank;



	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel11;
private: System::Windows::Forms::PictureBox^  BobMine;

	private: System::Windows::Forms::PictureBox^  ElsaMine;
	private: System::Windows::Forms::PictureBox^  SelbastienMine;



	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel12;
private: System::Windows::Forms::PictureBox^  BobSaloon;

	private: System::Windows::Forms::PictureBox^  ElsaSaloon;
	private: System::Windows::Forms::PictureBox^  SelbastienSaloon;







	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm1::typeid));
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel9 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->BobHouse = (gcnew System::Windows::Forms::PictureBox());
			this->ElsaHouse = (gcnew System::Windows::Forms::PictureBox());
			this->SelbastienHouse = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel10 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->BobBank = (gcnew System::Windows::Forms::PictureBox());
			this->ElsaBank = (gcnew System::Windows::Forms::PictureBox());
			this->SelbastienBank = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel11 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->BobMine = (gcnew System::Windows::Forms::PictureBox());
			this->ElsaMine = (gcnew System::Windows::Forms::PictureBox());
			this->SelbastienMine = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel12 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->BobSaloon = (gcnew System::Windows::Forms::PictureBox());
			this->ElsaSaloon = (gcnew System::Windows::Forms::PictureBox());
			this->SelbastienSaloon = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel7 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel8 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->tableLayoutPanel5 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel6 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel4 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->richTextBox3 = (gcnew System::Windows::Forms::RichTextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->applicationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lancerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->quitterToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->agentsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modifierLesÉtatsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tableLayoutPanel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			this->tableLayoutPanel9->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobHouse))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaHouse))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienHouse))->BeginInit();
			this->tableLayoutPanel10->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobBank))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaBank))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienBank))->BeginInit();
			this->tableLayoutPanel11->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobMine))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaMine))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienMine))->BeginInit();
			this->tableLayoutPanel12->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobSaloon))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaSaloon))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienSaloon))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->tableLayoutPanel1->SuspendLayout();
			this->tableLayoutPanel7->SuspendLayout();
			this->tableLayoutPanel8->SuspendLayout();
			this->tableLayoutPanel5->SuspendLayout();
			this->tableLayoutPanel6->SuspendLayout();
			this->tableLayoutPanel3->SuspendLayout();
			this->tableLayoutPanel4->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->ColumnCount = 5;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				24.57386F)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				10.79545F)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				25.28409F)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				15.48295F)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				23.72159F)));
			this->tableLayoutPanel2->Controls->Add(this->pictureBox1, 4, 1);
			this->tableLayoutPanel2->Controls->Add(this->pictureBox2, 2, 0);
			this->tableLayoutPanel2->Controls->Add(this->pictureBox3, 0, 1);
			this->tableLayoutPanel2->Controls->Add(this->tableLayoutPanel9, 2, 1);
			this->tableLayoutPanel2->Controls->Add(this->tableLayoutPanel10, 4, 2);
			this->tableLayoutPanel2->Controls->Add(this->tableLayoutPanel11, 0, 2);
			this->tableLayoutPanel2->Controls->Add(this->tableLayoutPanel12, 2, 3);
			this->tableLayoutPanel2->Controls->Add(this->pictureBox4, 2, 2);
			this->tableLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel2->Location = System::Drawing::Point(0, 24);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RowCount = 4;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25.95238F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 30.95238F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 29.51168F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.01274F)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(704, 471);
			this->tableLayoutPanel2->TabIndex = 7;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(539, 132);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(162, 123);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(252, 9);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(172, 102);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox2->TabIndex = 1;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(3, 132);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(167, 123);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox3->TabIndex = 2;
			this->pictureBox3->TabStop = false;
			// 
			// tableLayoutPanel9
			// 
			this->tableLayoutPanel9->ColumnCount = 3;
			this->tableLayoutPanel9->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel9->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel9->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel9->Controls->Add(this->BobHouse, 0, 0);
			this->tableLayoutPanel9->Controls->Add(this->ElsaHouse, 1, 0);
			this->tableLayoutPanel9->Controls->Add(this->SelbastienHouse, 2, 0);
			this->tableLayoutPanel9->Location = System::Drawing::Point(252, 124);
			this->tableLayoutPanel9->Name = L"tableLayoutPanel9";
			this->tableLayoutPanel9->RowCount = 1;
			this->tableLayoutPanel9->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel9->Size = System::Drawing::Size(172, 62);
			this->tableLayoutPanel9->TabIndex = 4;
			// 
			// BobHouse
			// 
			this->BobHouse->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BobHouse->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"BobHouse.Image")));
			this->BobHouse->Location = System::Drawing::Point(3, 3);
			this->BobHouse->Name = L"BobHouse";
			this->BobHouse->Size = System::Drawing::Size(51, 56);
			this->BobHouse->TabIndex = 0;
			this->BobHouse->TabStop = false;
			this->BobHouse->Visible = false;
			// 
			// ElsaHouse
			// 
			this->ElsaHouse->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ElsaHouse->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ElsaHouse.Image")));
			this->ElsaHouse->Location = System::Drawing::Point(60, 3);
			this->ElsaHouse->Name = L"ElsaHouse";
			this->ElsaHouse->Size = System::Drawing::Size(51, 56);
			this->ElsaHouse->TabIndex = 1;
			this->ElsaHouse->TabStop = false;
			// 
			// SelbastienHouse
			// 
			this->SelbastienHouse->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SelbastienHouse->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"SelbastienHouse.Image")));
			this->SelbastienHouse->Location = System::Drawing::Point(117, 3);
			this->SelbastienHouse->Name = L"SelbastienHouse";
			this->SelbastienHouse->Size = System::Drawing::Size(52, 56);
			this->SelbastienHouse->TabIndex = 2;
			this->SelbastienHouse->TabStop = false;
			this->SelbastienHouse->Visible = false;
			// 
			// tableLayoutPanel10
			// 
			this->tableLayoutPanel10->ColumnCount = 3;
			this->tableLayoutPanel10->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel10->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel10->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel10->Controls->Add(this->BobBank, 0, 0);
			this->tableLayoutPanel10->Controls->Add(this->ElsaBank, 1, 0);
			this->tableLayoutPanel10->Controls->Add(this->SelbastienBank, 2, 0);
			this->tableLayoutPanel10->Location = System::Drawing::Point(539, 269);
			this->tableLayoutPanel10->Name = L"tableLayoutPanel10";
			this->tableLayoutPanel10->RowCount = 1;
			this->tableLayoutPanel10->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel10->Size = System::Drawing::Size(162, 62);
			this->tableLayoutPanel10->TabIndex = 5;
			// 
			// BobBank
			// 
			this->BobBank->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BobBank->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"BobBank.Image")));
			this->BobBank->Location = System::Drawing::Point(3, 3);
			this->BobBank->Name = L"BobBank";
			this->BobBank->Size = System::Drawing::Size(48, 56);
			this->BobBank->TabIndex = 0;
			this->BobBank->TabStop = false;
			this->BobBank->Visible = false;
			// 
			// ElsaBank
			// 
			this->ElsaBank->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ElsaBank->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ElsaBank.Image")));
			this->ElsaBank->Location = System::Drawing::Point(57, 3);
			this->ElsaBank->Name = L"ElsaBank";
			this->ElsaBank->Size = System::Drawing::Size(48, 56);
			this->ElsaBank->TabIndex = 1;
			this->ElsaBank->TabStop = false;
			this->ElsaBank->Visible = false;
			// 
			// SelbastienBank
			// 
			this->SelbastienBank->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SelbastienBank->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"SelbastienBank.Image")));
			this->SelbastienBank->Location = System::Drawing::Point(111, 3);
			this->SelbastienBank->Name = L"SelbastienBank";
			this->SelbastienBank->Size = System::Drawing::Size(48, 56);
			this->SelbastienBank->TabIndex = 2;
			this->SelbastienBank->TabStop = false;
			this->SelbastienBank->Visible = false;
			// 
			// tableLayoutPanel11
			// 
			this->tableLayoutPanel11->ColumnCount = 3;
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel11->Controls->Add(this->BobMine, 0, 0);
			this->tableLayoutPanel11->Controls->Add(this->ElsaMine, 1, 0);
			this->tableLayoutPanel11->Controls->Add(this->SelbastienMine, 2, 0);
			this->tableLayoutPanel11->Location = System::Drawing::Point(3, 269);
			this->tableLayoutPanel11->Name = L"tableLayoutPanel11";
			this->tableLayoutPanel11->RowCount = 1;
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				62)));
			this->tableLayoutPanel11->Size = System::Drawing::Size(167, 62);
			this->tableLayoutPanel11->TabIndex = 6;
			// 
			// BobMine
			// 
			this->BobMine->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BobMine->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"BobMine.Image")));
			this->BobMine->Location = System::Drawing::Point(3, 3);
			this->BobMine->Name = L"BobMine";
			this->BobMine->Size = System::Drawing::Size(49, 56);
			this->BobMine->TabIndex = 0;
			this->BobMine->TabStop = false;
			this->BobMine->Visible = false;
			// 
			// ElsaMine
			// 
			this->ElsaMine->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ElsaMine->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ElsaMine.Image")));
			this->ElsaMine->Location = System::Drawing::Point(58, 3);
			this->ElsaMine->Name = L"ElsaMine";
			this->ElsaMine->Size = System::Drawing::Size(49, 56);
			this->ElsaMine->TabIndex = 1;
			this->ElsaMine->TabStop = false;
			this->ElsaMine->Visible = false;
			// 
			// SelbastienMine
			// 
			this->SelbastienMine->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SelbastienMine->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"SelbastienMine.Image")));
			this->SelbastienMine->Location = System::Drawing::Point(113, 3);
			this->SelbastienMine->Name = L"SelbastienMine";
			this->SelbastienMine->Size = System::Drawing::Size(51, 56);
			this->SelbastienMine->TabIndex = 2;
			this->SelbastienMine->TabStop = false;
			this->SelbastienMine->Visible = false;
			// 
			// tableLayoutPanel12
			// 
			this->tableLayoutPanel12->ColumnCount = 3;
			this->tableLayoutPanel12->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel12->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel12->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel12->Controls->Add(this->BobSaloon, 0, 0);
			this->tableLayoutPanel12->Controls->Add(this->ElsaSaloon, 1, 0);
			this->tableLayoutPanel12->Controls->Add(this->SelbastienSaloon, 2, 0);
			this->tableLayoutPanel12->Dock = System::Windows::Forms::DockStyle::Top;
			this->tableLayoutPanel12->Location = System::Drawing::Point(252, 407);
			this->tableLayoutPanel12->Name = L"tableLayoutPanel12";
			this->tableLayoutPanel12->RowCount = 1;
			this->tableLayoutPanel12->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel12->Size = System::Drawing::Size(172, 61);
			this->tableLayoutPanel12->TabIndex = 7;
			// 
			// BobSaloon
			// 
			this->BobSaloon->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BobSaloon->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"BobSaloon.Image")));
			this->BobSaloon->Location = System::Drawing::Point(3, 3);
			this->BobSaloon->Name = L"BobSaloon";
			this->BobSaloon->Size = System::Drawing::Size(51, 55);
			this->BobSaloon->TabIndex = 0;
			this->BobSaloon->TabStop = false;
			this->BobSaloon->Visible = false;
			// 
			// ElsaSaloon
			// 
			this->ElsaSaloon->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ElsaSaloon->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ElsaSaloon.Image")));
			this->ElsaSaloon->Location = System::Drawing::Point(60, 3);
			this->ElsaSaloon->Name = L"ElsaSaloon";
			this->ElsaSaloon->Size = System::Drawing::Size(51, 55);
			this->ElsaSaloon->TabIndex = 1;
			this->ElsaSaloon->TabStop = false;
			this->ElsaSaloon->Visible = false;
			// 
			// SelbastienSaloon
			// 
			this->SelbastienSaloon->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SelbastienSaloon->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"SelbastienSaloon.Image")));
			this->SelbastienSaloon->Location = System::Drawing::Point(117, 3);
			this->SelbastienSaloon->Name = L"SelbastienSaloon";
			this->SelbastienSaloon->Size = System::Drawing::Size(52, 55);
			this->SelbastienSaloon->TabIndex = 2;
			this->SelbastienSaloon->TabStop = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(252, 269);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(172, 132);
			this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox4->TabIndex = 3;
			this->pictureBox4->TabStop = false;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				20)));
			this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel7, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel5, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel3, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 495);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(704, 204);
			this->tableLayoutPanel1->TabIndex = 6;
			this->tableLayoutPanel1->TabStop = true;
			// 
			// tableLayoutPanel7
			// 
			this->tableLayoutPanel7->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->tableLayoutPanel7->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Outset;
			this->tableLayoutPanel7->ColumnCount = 1;
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel7->Controls->Add(this->tableLayoutPanel8, 0, 0);
			this->tableLayoutPanel7->Controls->Add(this->richTextBox2, 0, 1);
			this->tableLayoutPanel7->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel7->Location = System::Drawing::Point(471, 3);
			this->tableLayoutPanel7->Name = L"tableLayoutPanel7";
			this->tableLayoutPanel7->RowCount = 2;
			this->tableLayoutPanel7->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 13.26531F)));
			this->tableLayoutPanel7->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 86.7347F)));
			this->tableLayoutPanel7->Size = System::Drawing::Size(230, 198);
			this->tableLayoutPanel7->TabIndex = 5;
			// 
			// tableLayoutPanel8
			// 
			this->tableLayoutPanel8->ColumnCount = 2;
			this->tableLayoutPanel8->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				28.63636F)));
			this->tableLayoutPanel8->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				71.36364F)));
			this->tableLayoutPanel8->Controls->Add(this->label3, 0, 0);
			this->tableLayoutPanel8->Controls->Add(this->label4, 1, 0);
			this->tableLayoutPanel8->Dock = System::Windows::Forms::DockStyle::Top;
			this->tableLayoutPanel8->Location = System::Drawing::Point(5, 5);
			this->tableLayoutPanel8->Name = L"tableLayoutPanel8";
			this->tableLayoutPanel8->RowCount = 1;
			this->tableLayoutPanel8->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 39)));
			this->tableLayoutPanel8->Size = System::Drawing::Size(220, 19);
			this->tableLayoutPanel8->TabIndex = 0;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Dock = System::Windows::Forms::DockStyle::Top;
			this->label3->Location = System::Drawing::Point(3, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(56, 13);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Selbastien";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Dock = System::Windows::Forms::DockStyle::Top;
			this->label4->Location = System::Drawing::Point(65, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(152, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"OrderADrinkAndBottomsUp";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// richTextBox2
			// 
			this->richTextBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox2->Location = System::Drawing::Point(5, 32);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->ReadOnly = true;
			this->richTextBox2->ShowSelectionMargin = true;
			this->richTextBox2->Size = System::Drawing::Size(220, 161);
			this->richTextBox2->TabIndex = 1;
			this->richTextBox2->Text = L"";
			// 
			// tableLayoutPanel5
			// 
			this->tableLayoutPanel5->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->tableLayoutPanel5->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Outset;
			this->tableLayoutPanel5->ColumnCount = 1;
			this->tableLayoutPanel5->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel5->Controls->Add(this->tableLayoutPanel6, 0, 0);
			this->tableLayoutPanel5->Controls->Add(this->richTextBox1, 0, 1);
			this->tableLayoutPanel5->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel5->Location = System::Drawing::Point(237, 3);
			this->tableLayoutPanel5->Name = L"tableLayoutPanel5";
			this->tableLayoutPanel5->RowCount = 2;
			this->tableLayoutPanel5->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 13.77551F)));
			this->tableLayoutPanel5->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 86.22449F)));
			this->tableLayoutPanel5->Size = System::Drawing::Size(228, 198);
			this->tableLayoutPanel5->TabIndex = 4;
			// 
			// tableLayoutPanel6
			// 
			this->tableLayoutPanel6->ColumnCount = 2;
			this->tableLayoutPanel6->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				24.32432F)));
			this->tableLayoutPanel6->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				75.67567F)));
			this->tableLayoutPanel6->Controls->Add(this->label2, 0, 0);
			this->tableLayoutPanel6->Controls->Add(this->label5, 1, 0);
			this->tableLayoutPanel6->Dock = System::Windows::Forms::DockStyle::Top;
			this->tableLayoutPanel6->Location = System::Drawing::Point(5, 5);
			this->tableLayoutPanel6->Name = L"tableLayoutPanel6";
			this->tableLayoutPanel6->RowCount = 1;
			this->tableLayoutPanel6->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 39)));
			this->tableLayoutPanel6->Size = System::Drawing::Size(218, 20);
			this->tableLayoutPanel6->TabIndex = 0;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Dock = System::Windows::Forms::DockStyle::Top;
			this->label2->Location = System::Drawing::Point(3, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(47, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Elsa";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Dock = System::Windows::Forms::DockStyle::Top;
			this->label5->Location = System::Drawing::Point(56, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(159, 13);
			this->label5->TabIndex = 1;
			this->label5->Text = L"DoHouseWork";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox1->Location = System::Drawing::Point(5, 33);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->ShowSelectionMargin = true;
			this->richTextBox1->Size = System::Drawing::Size(218, 160);
			this->richTextBox1->TabIndex = 1;
			this->richTextBox1->Text = L"";
			// 
			// tableLayoutPanel3
			// 
			this->tableLayoutPanel3->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->tableLayoutPanel3->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Outset;
			this->tableLayoutPanel3->ColumnCount = 1;
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel3->Controls->Add(this->tableLayoutPanel4, 0, 0);
			this->tableLayoutPanel3->Controls->Add(this->richTextBox3, 0, 1);
			this->tableLayoutPanel3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel3->Location = System::Drawing::Point(3, 3);
			this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
			this->tableLayoutPanel3->RowCount = 2;
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28571F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 85.71429F)));
			this->tableLayoutPanel3->Size = System::Drawing::Size(228, 198);
			this->tableLayoutPanel3->TabIndex = 3;
			// 
			// tableLayoutPanel4
			// 
			this->tableLayoutPanel4->ColumnCount = 2;
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				24.32432F)));
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				75.67567F)));
			this->tableLayoutPanel4->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel4->Controls->Add(this->label6, 1, 0);
			this->tableLayoutPanel4->Dock = System::Windows::Forms::DockStyle::Top;
			this->tableLayoutPanel4->Location = System::Drawing::Point(5, 5);
			this->tableLayoutPanel4->Name = L"tableLayoutPanel4";
			this->tableLayoutPanel4->RowCount = 1;
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 107)));
			this->tableLayoutPanel4->Size = System::Drawing::Size(218, 21);
			this->tableLayoutPanel4->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Dock = System::Windows::Forms::DockStyle::Top;
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Bob";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Dock = System::Windows::Forms::DockStyle::Top;
			this->label6->Location = System::Drawing::Point(56, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(159, 13);
			this->label6->TabIndex = 1;
			this->label6->Text = L"GoHomeAndSleepTilRested";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// richTextBox3
			// 
			this->richTextBox3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox3->Location = System::Drawing::Point(5, 34);
			this->richTextBox3->Name = L"richTextBox3";
			this->richTextBox3->ReadOnly = true;
			this->richTextBox3->ShowSelectionMargin = true;
			this->richTextBox3->Size = System::Drawing::Size(218, 159);
			this->richTextBox3->TabIndex = 1;
			this->richTextBox3->Text = L"";
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->applicationToolStripMenuItem,
					this->agentsToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(704, 24);
			this->menuStrip1->TabIndex = 8;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// applicationToolStripMenuItem
			// 
			this->applicationToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->lancerToolStripMenuItem,
					this->quitterToolStripMenuItem
			});
			this->applicationToolStripMenuItem->Name = L"applicationToolStripMenuItem";
			this->applicationToolStripMenuItem->Size = System::Drawing::Size(80, 20);
			this->applicationToolStripMenuItem->Text = L"Application";
			// 
			// lancerToolStripMenuItem
			// 
			this->lancerToolStripMenuItem->Name = L"lancerToolStripMenuItem";
			this->lancerToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->lancerToolStripMenuItem->Text = L"Lancer";
			this->lancerToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm1::lancerToolStripMenuItem_Click);
			// 
			// quitterToolStripMenuItem
			// 
			this->quitterToolStripMenuItem->Name = L"quitterToolStripMenuItem";
			this->quitterToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->quitterToolStripMenuItem->Text = L"Quitter";
			// 
			// agentsToolStripMenuItem
			// 
			this->agentsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->modifierLesÉtatsToolStripMenuItem });
			this->agentsToolStripMenuItem->Name = L"agentsToolStripMenuItem";
			this->agentsToolStripMenuItem->Size = System::Drawing::Size(56, 20);
			this->agentsToolStripMenuItem->Text = L"Agents";
			// 
			// modifierLesÉtatsToolStripMenuItem
			// 
			this->modifierLesÉtatsToolStripMenuItem->Name = L"modifierLesÉtatsToolStripMenuItem";
			this->modifierLesÉtatsToolStripMenuItem->Size = System::Drawing::Size(164, 22);
			this->modifierLesÉtatsToolStripMenuItem->Text = L"Modifier les états";
			this->modifierLesÉtatsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm1::modifierLesÉtatsToolStripMenuItem_Click);
			// 
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(704, 699);
			this->Controls->Add(this->tableLayoutPanel2);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MyForm1";
			this->tableLayoutPanel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			this->tableLayoutPanel9->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobHouse))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaHouse))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienHouse))->EndInit();
			this->tableLayoutPanel10->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobBank))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaBank))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienBank))->EndInit();
			this->tableLayoutPanel11->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobMine))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaMine))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienMine))->EndInit();
			this->tableLayoutPanel12->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BobSaloon))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ElsaSaloon))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SelbastienSaloon))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel7->ResumeLayout(false);
			this->tableLayoutPanel8->ResumeLayout(false);
			this->tableLayoutPanel8->PerformLayout();
			this->tableLayoutPanel5->ResumeLayout(false);
			this->tableLayoutPanel6->ResumeLayout(false);
			this->tableLayoutPanel6->PerformLayout();
			this->tableLayoutPanel3->ResumeLayout(false);
			this->tableLayoutPanel4->ResumeLayout(false);
			this->tableLayoutPanel4->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void lancerToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		
		// Thread for the agent Bob
		Thread ^threadBob = gcnew Thread(gcnew ThreadStart(this, &MyForm1::ThrFunc1));
		threadBob->Name = "Thread1";

		// Thread for the agent Elsa
		Thread ^threadElsa = gcnew Thread(gcnew ThreadStart(this, &MyForm1::ThrFunc2));
		threadElsa->Name = "Thread2";

		// Launch threads
		threadBob->Start();
		threadElsa->Start();

		//delete Elsa;
		//delete Selbastien;

		//wait for a keypress before exiting
		//PressAnyKeyToContinue();
	}
	private: System::Void modifierLesÉtatsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Form ^f = gcnew ModificationAgents(Bob, Elsa, Selbastien);
		f->Show();
	}
};
}

