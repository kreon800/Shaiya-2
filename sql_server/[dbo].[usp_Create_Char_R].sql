USE [PS_GameData]
GO
/****** Object:  StoredProcedure [dbo].[usp_Create_Char_R]    Script Date: 2/1/2021 4:45:16 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER OFF
GO

ALTER Proc [dbo].[usp_Create_Char_R]

@ServerID tinyint,
@UserID varchar(12),
@UserUID int,
@CharName varchar(18),
@Slot tinyint,
@Family tinyint,
@Grow tinyint,
@Hair tinyint,
@Face tinyint,
@Size tinyint,
@Job tinyint,
@Sex tinyint,
@Level smallint,
@Statpoint smallint,
@Skillpoint smallint,
@Str smallint,
@Dex smallint,
@Rec smallint,
@Int smallint,
@Luc smallint,
@Wis smallint,
@Hp smallint,
@Mp smallint,
@Sp smallint,
@Map smallint,
@Dir smallint,
@Exp int,
@Money int,
@Posx real,
@Posy real,
@Posz real,
@Hg smallint,
@Vg smallint,
@Cg tinyint,
@Og tinyint,
@Ig tinyint,
@CharID int = 0,
@NameCnt tinyint = 0

AS

SET NOCOUNT ON

DECLARE @Ret int

SET @CharName = LTRIM(RTRIM(REPLACE(@CharName, char(39), char(32))))
SET @SkillPoint = 5
SET @Ret = 0

DECLARE @Regex AS int

SET @NameCnt = (SELECT ISNULL(COUNT(*), 0) FROM Chars WHERE CharName = @CharName AND Del = 0)
SET @Regex = PATINDEX('%[^a-zA-Z0-9._-]%', @CharName) 

IF @Regex > 0
RETURN -1

IF @NameCnt <> 0
BEGIN
	RETURN -2
END

ELSE
BEGIN
	IF EXISTS (SELECT CharID FROM Chars WHERE CharName = @CharName AND Del = 1 AND [Level] > 10 AND DeleteDate > DATEADD(dd, -7, GETDATE()) )
	BEGIN
		RETURN -2
	END

	BEGIN TRANSACTION

	INSERT INTO Chars(ServerID, UserID, UserUID, CharName, Slot, Family, Grow, 
	Hair, Face, [Size], Job, Sex, [Level], StatPoint, SkillPoint, 
	[Str], Dex, Rec, [Int], Luc, Wis, HP, MP, SP, Map, Dir, [Exp], [Money], 
	PosX, PosY, Posz, Hg, Vg, Cg, Og, Ig, RenameCnt, RemainTime)

	VALUES(@ServerID, @UserID, @UserUID, @CharName, @Slot, @Family, @Grow, 
	@Hair, @Face, @Size, @Job, @Sex, @Level, @StatPoint, @SkillPoint, 
	@Str, @Dex, @Rec, @Int, @Luc, @Wis, @HP, @MP, @SP, @Map, @Dir, @Exp, @Money, 
	@PosX, @PosY, @Posz, @Hg, @Vg, @Cg, @Og, @Ig, 0, 0)

	IF @@ERROR = 0
	BEGIN
		COMMIT TRANSACTION
	END
	
	ELSE
	BEGIN
		ROLLBACK TRANSACTION
		RETURN -1
	END

	SET @CharID = IDENT_CURRENT('Chars')
END

RETURN @CharID

SET NOCOUNT OFF
