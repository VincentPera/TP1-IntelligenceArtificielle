#pragma once

#include "Miner.h"
#include "Drunk.h"
#include "MinersWife.h"

namespace Projet1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Description résumée de ModificationAgents
	/// </summary>
	public ref class ModificationAgents : public System::Windows::Forms::Form
	{
	public:
		ModificationAgents(Miner* bob, MinersWife* elsa, Drunk* selbastien)
		{
			agentBob = bob;
			agentElsa = elsa;
			agentSelbastien = selbastien;

			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~ModificationAgents()
		{
			if (components)
			{
				delete components;
			}
		}
	private: Miner* agentBob;
	private: MinersWife* agentElsa;
	private: Drunk* agentSelbastien;

	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	protected:
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::ComboBox^  comboBox2;

	private: System::Windows::Forms::ComboBox^  comboBox1;


	private: System::Windows::Forms::ComboBox^  comboBox3;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;


	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;


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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ModificationAgents::typeid));
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->flowLayoutPanel1->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->button1);
			this->flowLayoutPanel1->Controls->Add(this->button2);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->flowLayoutPanel1->FlowDirection = System::Windows::Forms::FlowDirection::RightToLeft;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 189);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(293, 29);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(215, 3);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Valider";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ModificationAgents::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(134, 3);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Annuler";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ModificationAgents::button2_Click);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				57)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				26.69492F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				73.30508F)));
			this->tableLayoutPanel1->Controls->Add(this->comboBox2, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->comboBox1, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->comboBox3, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->label1, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label3, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox3, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox2, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 62)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 45.66929F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 54.33071F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(293, 189);
			this->tableLayoutPanel1->TabIndex = 1;
			// 
			// comboBox2
			// 
			this->comboBox2->AllowDrop = true;
			this->comboBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->comboBox2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"DoHousework" });
			this->comboBox2->Location = System::Drawing::Point(123, 80);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(167, 21);
			this->comboBox2->TabIndex = 1;
			// 
			// comboBox1
			// 
			this->comboBox1->AllowDrop = true;
			this->comboBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
				L"GoHomeAndSleepTillRested", L"EnterMineAndDigForNuggets",
					L"VisitBankAndDepositGold", L"QuenchThirst", L"FightForPride", L"RoundForEveryone"
			});
			this->comboBox1->Location = System::Drawing::Point(123, 20);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(167, 21);
			this->comboBox1->TabIndex = 1;
			// 
			// comboBox3
			// 
			this->comboBox3->AllowDrop = true;
			this->comboBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->comboBox3->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"OrderADrinkAndBottomsUp", L"DrukenRage", L"DanceOnTheTable" });
			this->comboBox3->Location = System::Drawing::Point(123, 143);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(167, 21);
			this->comboBox3->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(60, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(57, 26);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Etat Initial Bob";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(60, 77);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(57, 26);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Etat Initial Elsa";
			this->label2->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(60, 141);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(57, 26);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Etat Initial SelBastien";
			this->label3->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(3, 122);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(51, 64);
			this->pictureBox3->TabIndex = 9;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(3, 65);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(51, 51);
			this->pictureBox1->TabIndex = 10;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(3, 3);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(51, 56);
			this->pictureBox2->TabIndex = 11;
			this->pictureBox2->TabStop = false;
			// 
			// ModificationAgents
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(293, 218);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->flowLayoutPanel1);
			this->Name = L"ModificationAgents";
			this->Text = L"ModificationAgents";
			this->flowLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	ModificationAgents::Close();
}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	// switch for Bob
	switch (this->comboBox1->SelectedIndex)
	{
	case 0: agentBob->ChangeState(GoHomeAndSleepTilRested::Instance()); break;
	case 1: agentBob->ChangeState(EnterMineAndDigForNugget::Instance()); break;
	case 2: agentBob->ChangeState(VisitBankAndDepositGold::Instance()); break;
	case 3: agentBob->ChangeState(QuenchThirst::Instance()); break;
	case 4: agentBob->ChangeState(FightForPride::Instance()); break;
	case 5: agentBob->ChangeState(RoundForEveryone::Instance()); break;
	default:
		break;
	}
	// switch for Elsa
	switch (this->comboBox2->SelectedIndex)
	{
	case 0: agentElsa->ChangeState(DoHouseWork::Instance()); break;
	default:
		break;
	}
	// switch for Selbastien
	switch (this->comboBox3->SelectedIndex)
	{
	case 0: agentSelbastien->ChangeState(OrderADrinkAndBottomUp::Instance()); break;
	case 1: agentSelbastien->ChangeState(DrunkenRage::Instance()); break;
	case 2: agentSelbastien->ChangeState(DanceOnTheTable::Instance()); break;
	default:
		break;
	}

	this->Hide();
}
};
}
