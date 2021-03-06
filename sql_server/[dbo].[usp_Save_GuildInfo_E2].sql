USE [PS_GameData]
GO
/****** Object:  StoredProcedure [dbo].[usp_Save_GuildInfo_E2]    Script Date: 2/1/2021 5:05:36 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

ALTER  Proc [dbo].[usp_Save_GuildInfo_E2]

@GuildID int,
@GuildPoint int,
@Etin int,
@Remark	varchar(64),
@BuyHouse tinyint,
@EtinReturnCnt int,
@KeepEtin int,
@Rank tinyint,
@UseHouse tinyint

AS

SET NOCOUNT ON

DECLARE @OldGuildPoint int

SET @Remark = LTRIM(RTRIM(REPLACE(@Remark, char(39), char(32))))

BEGIN TRAN

SELECT @OldGuildPoint = GuildPoint FROM Guilds WHERE GuildID = @GuildID

IF @GuildPoint <> @OldGuildPoint
BEGIN
	UPDATE Guilds SET GuildPoint = @GuildPoint WHERE GuildID = @GuildID

	IF @@ERROR <> 0
	BEGIN	
		ROLLBACK TRAN
		RETURN -1
	END
END

IF EXISTS (SELECT GuildID FROM GuildDetails WHERE GuildID = @GuildID)
BEGIN
	UPDATE GuildDetails 
	SET UseHouse = @UseHouse, BuyHouse = @BuyHouse, [Rank] = @Rank, Etin = @Etin, EtinReturnCnt = @EtinReturnCnt, KeepEtin = @KeepEtin, Remark = @Remark
	WHERE GuildID = @GuildID
END

ELSE
BEGIN
	INSERT INTO GuildDetails(GuildID, UseHouse, BuyHouse, [Rank], Etin, EtinReturnCnt, KeepEtin, Remark) 
	VALUES(@GuildID, @UseHouse, @BuyHouse, @Rank, @Etin, @EtinReturnCnt, @KeepEtin, @Remark)
END

IF @@ERROR = 0
BEGIN
	COMMIT TRAN
	RETURN 1
END

ELSE
BEGIN
	ROLLBACK TRAN
	RETURN -1
END

SET NOCOUNT OFF
